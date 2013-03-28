// Copyright (c) 2013, Rob Gray (www.robgray.com)
//  
// Permission to use, copy, modify, and/or distribute this software for  
// any purpose with or without fee is hereby granted, provided that the  
// above copyright notice and this permission notice appear in all copies.  
// 
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
// SOFTWARE.  
//
// History:
//	17mar13:RG:Initial GIT upload
//	
///////////////////////////////// hwserial.c ///////////////////////////////
//
//  
//
//
//

#include "LARD.h"

/////////////////////////////////////////////////////////////////
// Array of pointers to all created serial structures
//
// The first 6 entries are reserved for hardware UARTs, the next 4
// may be used for software serial
//
serialConnection * serialConnections [10];
serialConnection * uartConnections [N_UARTS];

uint32 serialAttachCallback (serialConnection * s, uint32 event_type, void (*callback_func)(struct _serialConnection *, uint32 e)) {

	VERIFY_OBJECT(s, OBJID_SERIAL_CONNECTION)

	if (event_type >= UART_CALLBACK_DUMMY) {
		SYS_ERROR (ERR_SERIAL_BAD_CALLBACK_FUNCTION | 1);
		return ERROR;
	}

	s->callback_funcs[event_type] = callback_func;

	return NOERROR;

}

serialConnection * serialCreate (uint8 port, uint32 bitrate, uint8 data_bits, uint8 parity, uint8 stop_bits,
		uint8 rx_buff_size, uint8 tx_buff_size) {

	LPC_UART0_Type * uart;
	serialConnection * s;
	uint8 temp;

	if (port > MAX_SERIAL_CONNECTIONS) {
		SYS_ERROR (ERR_SERIAL_BAD_PORT | 1);
		SYS_ERROR (ERR_SERIAL_INIT_FAILED);
		return (serialConnection *)ERROR;
	}

	/////////////////////////////////////////////////////////////////
	// Create the serial structure
	s = (void*)safeMalloc(sizeof (serialConnection));

	if (s == NULL) {
		SYS_ERROR (ERR_SERIAL_INIT_FAILED | 2);
		return (serialConnection *)ERROR;
	}

	serialConnections [port] = s;

	s->object_id 	 = OBJID_SERIAL_CONNECTION;
	s->not_object_id = ~OBJID_SERIAL_CONNECTION;

	s->port 			= port;
	s->hwFifoCount 		= 0;
	s->swFifoCount 		= 0;
	s->txState			= SERIAL_STATE_IDLE;
	s->sending_break 	= FALSE;
	s->delimBegin		= '<';
	s->delimEnd			= '>';

	/////////////////////////////////////////////////////////////////
	//
	switch (port) {
		case SERIAL_UART0:

			/////////////////////////////////////////////////////////////////
			// Setup default Rx and Tx pins
			serialSetUartPins (s, UART0_PINS_R1_T2);

			/////////////////////////////////////////////////////////////////
			// Disable this UART's interrupt for the duration
			NVIC_DisableIRQ(UART0_IRQn);

			/////////////////////////////////////////////////////////////////
			// Set a pointer to the UART structure. Note that UART0 and UART1 have different
			// structs but for the purposes of this function they are the same because
			// we are not using the RS-485 features of UART0
			uart = LPC_UART0;
			s->uart = uart;

			/////////////////////////////////////////////////////////////////
			// Release the UART's reset
			LPC_SYSCON->PRESETCTRL |= (0x1 << 2);		// Set UART0_RST_N bit

			/////////////////////////////////////////////////////////////////
			// Enable the UART's AHB clock
			LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << AHB_UART0);	// Set UART0 bit

			/////////////////////////////////////////////////////////////////
			// Set the UART's clock divider
			LPC_SYSCON->UART0CLKDIV = 0x1;  			// divide by 1, may be different later

			/////////////////////////////////////////////////////////////////
			// Get the clock divider for later use in bitrate calcs
			// Could use the constant 1 but maybe this will change later
			// so we just re-read the register
			s->clkDiv = LPC_SYSCON->UART0CLKDIV;
			break;

		case SERIAL_UART1:
			// NOTE: Comments as per the above UART0 code

			serialSetUartPins (s, UART1_PINS_R8_T9);

			NVIC_DisableIRQ(UART1_IRQn);
			uart = (LPC_UART0_Type*) LPC_UART1;

			LPC_SYSCON->PRESETCTRL |= (0x1 << 3);

			LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << AHB_UART1);

			LPC_SYSCON->UART1CLKDIV = 0x1;

			s->clkDiv = LPC_SYSCON->UART1CLKDIV;

			break;

		default:
			SYS_ERROR (ERR_SERIAL_INIT_FAILED | (port << 8) | 7);
			return (serialConnection *)ERROR;

	}

	/////////////////////////////////////////////////////////////////
	// Create Rx and Tx buffers
	if (rx_buff_size) {
		s->rxBuffer = fifo8Create(rx_buff_size);
		if (s->rxBuffer == NULL) {
			safeFree (s);
			SYS_ERROR (ERR_SERIAL_INIT_FAILED | 3);
			return (serialConnection *)ERROR;
		};
	} else {
		// TODO: this should be an error condition
		s->rxBuffer = NULL;
	}
	if (tx_buff_size) {
		s->txBuffer = fifo8Create(tx_buff_size);
		if (s->txBuffer == NULL) {
			safeFree (s);
			safeFree (s->rxBuffer);
			SYS_ERROR (ERR_SERIAL_INIT_FAILED | 4);
			return (serialConnection *)ERROR;
		}
	} else {
		s->txBuffer = NULL;
	}

	/////////////////////////////////////////////////////////////////
	//
	// Start with a blank slate in the Line Control Register
	//
	uart->LCR = 0;

	serialSetDataLength (s, data_bits);
	serialSetStopBits 	(s, stop_bits);
	serialSetParity 	(s, parity);
	serialSetBitRate	(s, bitrate);

	//////////////////////////////////////////////////////////////////
	//
	// Setup Fractional Divide Register
	//
	uart->FDR = 0x10; // no fractional div, prescaler = 1

	//////////////////////////////////////////////////////////////////
	//
	// Setup FIFOs
	//
	uart->FCR = (1 << 0) |		// Enable both FIFOs
			    (1 << 1) |		// Reset Rx FIFO
			    (1 << 2) |		// Reset Tx FIFO
			    (7 << 6);		// RX FIFO trigger level = 8 chars

	//////////////////////////////////////////////////////////////////
	//
	//	 Ensure a clean start, no data in either Tx or Rx FIFO.
	//
	while ((uart->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT)) ;
	while (uart->LSR & LSR_RDR)	temp = uart->RBR; // Dump data from Rx FIFO

	//////////////////////////////////////////////////////////////////
	//
	// Clear any line status bits
	//
	temp = uart->LSR;

	//////////////////////////////////////////////////////////////////
	//
	// Setup and enable UART interrupts
	//
	uart->IER = IER_RBR | IER_THRE | IER_RX;

	if (port == 0)
		NVIC_EnableIRQ(UART0_IRQn);
	else
		NVIC_EnableIRQ(UART1_IRQn);

	uartConnections [port] = s;

	return s;

}

/////////////////////////////////////////////////////////////////
//
// Array of logical pin numbers usable by the two UARTs
//
// UART0 can use 1/2 or 40/41 for RxD/Txd
// UART1 can use 8/9, 50/49 or 51/52 for RxD/Txd
//
// TODO: move to chips files
//
uint8 __uart_logical_pins[2][6] = {
/*   				Rxd		Txd		Rxd		Txd		Rxd		Txd */
/* UART0 */		   {1,		2,		40,		41,		ERROR,	ERROR},
/* UART1 */		   {8,		9,		50, 	49,		51,		52}
};


uint32 serialSetUartPins(serialConnection * s, uint32 location) {

	uint8 txPin;
	uint8 rxPin;

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	ASSERT_RETERR (location > 3, ERR_SERIAL_BAD_PORT);

	location <<= 1;
	rxPin = __uart_logical_pins[s->port][location];
	txPin = __uart_logical_pins[s->port][location+1];


	ASSERT_RETERR ((rxPin == ERROR) || (txPin == ERROR), ERR_SERIAL_BAD_LOCATION);

	TRY
		pinFunc (rxPin, (s->port == 0) ? FUNC_RXD0 : FUNC_RXD1);
		pinFunc (txPin, (s->port == 0) ? FUNC_TXD0 : FUNC_TXD1);
	CATCH_RETERR (ERR_RESOURCE_CLASH)

	s->txPin = txPin;
	s->rxPin = rxPin;

	TRY
		resourceRecordUsage (RESOURCE_PIN0 + txPin);
		resourceRecordUsage (RESOURCE_PIN0 + rxPin);
	CATCH (ERR_RESOURCE_CLASH)

	return NOERROR;
}

/////////////////////////////////////////////////////////////
//
// Non-user functions called by the packet object
//
INLINE void __serialSetDelimBegin (serialConnection * s, char d) {
	s->delimBegin = d;
}

INLINE void __serialSetDelimEnd (serialConnection * s, char d) {
	s->delimEnd	= d;
}

/////////////////////////////////////////////////////////////
// Sends 10 consecutive 0 bits, start, 8 x 0, and 0 parity.
void serialSendBreak (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	serialSetParity (s, UART_PARITY_FORCE0);
	serialWrite 	(s, 0x00);
	s->sending_break = TRUE;

}

void serialStartBreak (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->LCR |= LCR_BC;

}

void serialEndBreak (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->LCR &= ~LCR_BC;
}

uint32 serialTxIsIdle(serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

//	pinPulse(7);

	return s->txState == SERIAL_STATE_TRANSMITTING ? FALSE : TRUE;

}

INLINE uint32 serialTxInProgress(serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

//	pinPulse(7);

	return s->txState == SERIAL_STATE_TRANSMITTING ? TRUE : FALSE;

}

INLINE void serialWaitForTxToClear (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	while (s->txState == SERIAL_STATE_TRANSMITTING) ;

}

INLINE uint32 serialAvailable (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	return s->rxBuffer->nItems;
}

INLINE uint32 serialRead (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	return (uint32)fifo8Read (s->rxBuffer, NULL);

}
#if 1

INLINE uint32 serialGetMaxBufferLevel (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	return s->txBuffer->maxItems;
}

INLINE void serialResetMaxBufferLevel (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->txBuffer->maxItems = 0;
}

INLINE void serialPrintString (serialConnection * s, char * str) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

//	if (s->sending_break) serialSetParity (s, UART_PARITY_NONE);
//
//	while ((s->hwFifoCount < TX_FIFO_SIZE) && *str)
//		UART_FIFO_WRITE(s,*str++);

	while (*str) serialWrite (s, *str++);

}

INLINE void serialFifoWrite (serialConnection * s, uint8 b) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	UART_FIFO_WRITE(s,b);
}

uint32 serialWrite (serialConnection * s, uint8 b) {

	uint32 retval;

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	if (s->sending_break) serialSetParity (s, UART_PARITY_NONE);

	ATOMIC_START

	uartConnections[0]->txState =  SERIAL_STATE_TRANSMITTING;

	if (s->txBuffer != NULL) {
		//////////////////////////////////////////////////////
		// If the FIFO is full OR we already have bytes in the
		// buffer we add this one to the buffer
		if (s->txBuffer->nItems != 0 || s->hwFifoCount == TX_FIFO_SIZE) {
			//////////////////////////////////////////////////////
			// Either there are bytes in the buffer OR the
			// FIFO is full
			pin7pulse();
			retval = fifo8Write (s->txBuffer, b);
		} else {
			//////////////////////////////////////////////////////
			// There are no bytes in the buffer and the FIFO is
			// not full, so just write this one to the UART
			pin14pulse();
			UART_FIFO_WRITE(s,b);
			retval = 0;
		}
	} else {
		//////////////////////////////////////////////////////
		// The user has not defined a Tx buffer so all we can
		// do is write the byte directly to the FIFO. Tough luck
		// if it's already full
		UART_FIFO_WRITE(s,b);
	}

	ATOMIC_END

	return retval;
}


#else
void serialWrite (serialConnection * s, uint8 b) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	ATOMIC_START
	//////////////////////////////////////////////////////
	// If there are any bytes in the buffer then we must
	// add this byte to it.
	if (s->swFifoCount) {
		fifo8Write (s->txBuffer, b);
		s->swFifoCount++;
	} else {
		//////////////////////////////////////////////////////
		// If there are any free spots in the FIFO we can add
		// this byte to the FIFO
		if (s->hwFifoCount < TX_FIFO_SIZE) {
			s->uart->THR = b;
			s->hwFifoCount++;
		} else {
			//////////////////////////////////////////////////////
			// Otherwise it has to go into the buffer
			fifo8Write (s->txBuffer, b);
			s->swFifoCount++;
		}
	}
	ATOMIC_END

	uint32 x = LPC_UART0->FIFOLVL;
}

#endif
/////////////////////////////////////////////////////////////////
//
// Setup data bits
//
uint32 serialSetDataLength (serialConnection * s, uint8 data_bits) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	if (data_bits < 5 || data_bits > 8) {
		SYS_ERROR (ERR_SERIAL_BAD_DATA_BITS | data_bits);
		data_bits = UART_DATA_BITS_8;
	}
	s->uart->LCR &= 0b11111100;	// clear WLS bits
	s->uart->LCR |= data_bits - 5;
	s->data_bits = data_bits;

	return NOERROR;
}

/////////////////////////////////////////////////////////////////
//
// Setup stop bits
//
uint32 serialSetStopBits (serialConnection * s, uint8 stop_bits) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	if ((stop_bits != UART_STOP_BITS_1) && (stop_bits != UART_STOP_BITS_2)) {
		SYS_ERROR (ERR_SERIAL_BAD_STOP_BITS | stop_bits);
		stop_bits = UART_STOP_BITS_2;
	}
	s->uart->LCR &= 0b11111011;	// clear SBS bit
	s->uart->LCR |= stop_bits;
	s->stop_bits = stop_bits;

	return NOERROR;
}

//////////////////////////////////////////////////////////////////
//
// Setup bitrate
//
uint32 serialSetBitRate (serialConnection * s, uint32 rate) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	uint32 Fdiv;

	Fdiv = ((SystemCoreClock / s->clkDiv) / 16) / rate;

	s->uart->LCR |= LCR_DLAB;		// DLAB = 1
	s->uart->DLM = Fdiv / 256;
	s->uart->DLL = Fdiv % 256;
	s->uart->LCR &= ~LCR_DLAB; 	// DLAB = 0

	s->bitrate = rate;
	s->break_val = 1000000UL / rate * 10;

	return NOERROR;

}

//////////////////////////////////////////////////////////////////
//
// Setup parity
//
uint32 serialSetParity (serialConnection * s, uint8 parity) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	if ((parity != UART_PARITY_NONE) &&
		(parity != UART_PARITY_ODD) &&
		(parity != UART_PARITY_EVEN) &&
		(parity != UART_PARITY_FORCE1) &&
		(parity != UART_PARITY_FORCE0)) {

		SYS_ERROR (ERR_SERIAL_BAD_PARITY);
		parity = UART_PARITY_NONE;
	}
	s->uart->LCR &= 0b11000111;	// clear PE and PST bits
	s->uart->LCR |= parity;
	s->parity = parity;

	return NOERROR;
}

uint32 serialEnableParity (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->LCR |= LCR_PE;

	return NOERROR;
}

uint32 serialDisableParity (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->LCR &= ~LCR_PE;

	return NOERROR;
}

uint32 serialLoopbackOn (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->MCR |= (1 << 4);

	return NOERROR;
}

uint32 serialLoopbackOff (serialConnection * s) {

	VERIFY_OBJECT (s, OBJID_SERIAL_CONNECTION);

	s->uart->MCR &= ~(1 << 4);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////
//
// There is at least one char in Rx FIFO
//
void serial0_rlsFuncRDR (void) {
#if 0
	uint32 LSRval;

	if (eventFunctions[EVENT_UART0_RX_CHAR_AVAILABLE] != NULL) {
		(eventFunctions[EVENT_UART0_RX_CHAR_AVAILABLE]) ();
	} else {
		do  {
			LSRval = LPC_UART0->LSR;
			fifo16Write(serialConnections[0]->rxBuffer, (LSRval << 8) | LPC_UART0->RBR);
		} while (LSRval & LSR_RDR);
	}
#endif
}

/////////////////////////////////////////////////////////////////////
//
// Rx FIFO has reached trigger level
//
void serial0_rlsFuncRDA (void) {
//	uint32 LSRval;

//	pinHigh(7);
	if (eventFunctions[EVENT_UART0_RX_FIFO_TRIGLVL] != NULL) {
		(eventFunctions[EVENT_UART0_RX_FIFO_TRIGLVL]) ();
	} else {
		while (LPC_UART0->LSR & LSR_RDR)  {
//			LSRval = LPC_UART0->LSR;
//			pinPulse(15);
			fifo8Write(serialConnections[0]->rxBuffer, /*(LSRval << 8)*/ 0 | LPC_UART0->RBR);
		}
	}
//	pinLow(7);
}

/////////////////////////////////////////////////////////////////////
//
// Overrun error, there's a new character in the Rx shift register
// and the FIFO is full.
//
void serial0_rlsFuncOE (void) {

	if (eventFunctions[EVENT_UART0_RX_OVERRUN_ERR] != NULL)
		(eventFunctions[EVENT_UART0_RX_OVERRUN_ERR]) ();
	else {
//		UARTStatus0 = LSRval; 					// Read LSR will clear interrupt
//		serial0enqueueByte (LPC_UART0->RBR); 	// Save character in buffer
	}
}

/////////////////////////////////////////////////////////////////////
//
// Parity error, the character at the top of the FIFO has a parity error.
//
void serial0_rlsFuncPE (void) {

}

/////////////////////////////////////////////////////////////////////
//
// Framing error, the character at the top of the FIFO has a framing
// error, IE the stop bit was LOW.
//
void serial0_rlsFuncFE (void) {

}

/////////////////////////////////////////////////////////////////////
//
//
void serial0_rlsFuncBI (void) {

}

void serial0_rlsDummy (void) {

}

const voidFuncPtr serial0_rlsFunctions[32] = {
	//						BI	FE	PE	OE	RDR
	serial0_rlsDummy,	//	0	0	0	0	0
	serial0_rlsFuncRDA,	//	0	0	0	0	1
	serial0_rlsFuncOE,	//	0	0	0	1	0
	serial0_rlsFuncOE,	//	0	0	0	1	1
	serial0_rlsFuncRDA,	//	0	0	1	0	0
	serial0_rlsFuncRDA,	//	0	0	1	0	1
	serial0_rlsFuncOE,	//	0	0	1	1	0
	serial0_rlsFuncOE,	//	0	0	1	1	1
	serial0_rlsFuncFE,	//	0	1	0	0	0
	serial0_rlsFuncFE,	//	0	1	0	0	1
	serial0_rlsFuncOE,	//	0	1	0	1	0
	serial0_rlsFuncOE,	//	0	1	0	1	1
	serial0_rlsFuncFE,	//	0	1	1	0	0
	serial0_rlsFuncFE,	//	0	1	1	0	1
	serial0_rlsFuncOE,	//	0	1	1	1	0
	serial0_rlsFuncOE,	//	0	1	1	1	1
	serial0_rlsFuncBI,	//	1	0	0	0	0
	serial0_rlsFuncBI,	//	1	0	0	0	1
	serial0_rlsFuncBI,	//	1	0	0	1	0
	serial0_rlsFuncBI,	//	1	0	0	1	1
	serial0_rlsFuncBI,	//	1	0	1	0	0
	serial0_rlsFuncBI,	//	1	0	1	0	1
	serial0_rlsFuncBI,	//	1	0	1	1	0
	serial0_rlsFuncBI,	//	1	0	1	1	1
	serial0_rlsFuncBI,	//	1	1	0	0	0
	serial0_rlsFuncBI,	//	1	1	0	0	1
	serial0_rlsFuncBI,	//	1	1	0	1	0
	serial0_rlsFuncBI,	//	1	1	0	1	1
	serial0_rlsFuncBI,	//	1	1	1	0	0
	serial0_rlsFuncBI,	//	1	1	1	0	1
	serial0_rlsFuncBI,	//	1	1	1	1	0
	serial0_rlsFuncBI	//	1	1	1	1	1

};


void UART1_IRQHandler(void) {}          // UART1

void UART0_IRQHandler(void) {
	uint8	 LSRval;

	LSRval 	= LPC_UART0->IIR;	// dummy read to clear THRE interrupt
	LSRval 	= LPC_UART0->LSR;	// read to clear error flags in LSR

	// Call any Rx function
	(serial0_rlsFunctions[LSRval & 0b11111]) ();

	// Call the Tx function
	if (LSRval & (LSR_THRE | LSR_TEMT)) {

		if (uartConnections[0]->txBuffer == NULL) return;

		pin15high();
		uartConnections[0]->hwFifoCount = 0;

		switch (uartConnections[0]->txBuffer->nItems) {

			case 0:
				uartConnections[0]->txState = SERIAL_STATE_IDLE;
				break;

			case 1:
				uartConnections[0]->hwFifoCount = 1;
				pinPulse(14);
				LPC_UART0->THR = fifo8Read(uartConnections[0]->txBuffer, NULL);
				break;

			default:
				for (int i = 0; i < TX_FIFO_SIZE; i++) {
					if (uartConnections[0]->txBuffer->nItems == 1) {
						pin15low();
						return;
					}
					uartConnections[0]->hwFifoCount++;
					pin14pulse();
					LPC_UART0->THR = fifo8Read(uartConnections[0]->txBuffer, NULL);
				}
				break;
		}
	}
	pin15low();

}

#if 0
void UART0_IRQHandler(void) {
	uint8_t IIRValue, LSRval;
	uint8_t Dummy = Dummy;

	//	__disable_irq();

	IIRValue = LPC_UART0->IIR;

//	IIRValue >>= 1; 		// remove pending bit in IIR
	IIRValue &= 0x0F; 		// check bit 1~3, interrupt identification

	switch (IIRValue) {
		case IIR_RLS:
			LSRval = LPC_UART0->LSR;
			(serial0_rlsFunctions[LSRval & 0b11111]) ();
			break;

#if 0
			if (LSRval & (LSR_OE | LSR_PE | LSR_FE | LSR_BI)) {
				UARTStatus0 = LSRval; // Read LSR will clear the interrupt
				Dummy = LPC_UART0->RBR; // Dummy read on RX to clear interrupt, then bail out
				return;
			}
			if (LSRval & LSR_RDR) { /* Receive Data Ready */
				// No errors save into the data buffer.

				UARTBuffer0[UARTCount0++] = LPC_UART0->RBR; // Note: read RBR will clear the interrupt
				if (UARTCount0 == BUFSIZE) {
					UARTCount0 = 0; /* buffer overflow */
				}
			}
#endif
			break;

		case IIR_RDA:		// Receive Data Available
			serial0_rlsFuncRDA();
			break;

		case IIR_CTI:		// Character timeout indicator
			break;

		case IIR_THRE:		// FIFO is empty (still one byte in SR though)
			LSRval = LPC_UART0->LSR;

			// Check status in the LSR to see if valid data in U0THR or not
			if (LSRval & LSR_THRE) {
//				UARTTxEmpty0 = 1;
			} else {
//				UARTTxEmpty0 = 0;
			}
			break;

	}

	//	__enable_irq();

}
#endif

#if 0
void uartinit(uint32_t brate)
{
    uint32_t regVal, Fdiv;

    NVIC_DisableIRQ(UART0_IRQn);

    //Enable Pins 0_1 and 0_2 for UART0
    LPC_IOCON->PIO0_1 &= ~0x07;    /* UART0 I/O config */
    LPC_IOCON->PIO0_1 |= 0x02;     /* UART0 RXD LOC0 */
    LPC_IOCON->PIO0_2 &= ~0x07;
    LPC_IOCON->PIO0_2 |= 0x02;     /* UART0 TXD LOC0 */

    /* Enable UART 0 clock */
    LPC_SYSCON->PRESETCTRL |= (0x1<<2);
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<12);
    LPC_SYSCON->UART0CLKDIV = 0x1;     /* divided by 1 */

    LPC_UART0->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
    regVal = LPC_SYSCON->UART0CLKDIV;
    Fdiv = ((SystemCoreClock/regVal)/16)/brate ;    /*baud rate */

    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;        /* DLAB = 0 */
    LPC_UART0->FDR = 0x10;        /* set to default value: 0x10 */
    LPC_UART0->FCR = 0x07;        /* Enable and reset TX and RX FIFO. */

    /* Read to clear the line status. */
    regVal = LPC_UART0->LSR;

    /* Ensure a clean start, no data in either TX or RX FIFO. */
    while ( (LPC_UART0->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );

    NVIC_EnableIRQ(UART0_IRQn); //Re-enable interrupt after changes

    //LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;    /* Enable UART interrupt */
}
#endif
