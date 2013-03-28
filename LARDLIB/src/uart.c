/****************************************************************************
 *   $Id:: uart.c 6782 2011-03-11 21:53:54Z nxp28548                        $
 *   Project: NXP LPC122x UART example
 *
 *   Description:
 *     This file contains UART code example which include UART
 *     initialization, UART interrupt handler, and related APIs for
 *     UART access.
 *
*/

#include "LARD.h"

volatile uint32_t UARTStatus0;
volatile uint8_t UARTTxEmpty0 = 1;
volatile uint8_t UARTBuffer0[BUFSIZE];
volatile uint32_t UARTCount0 = 0;

volatile uint32_t UARTStatus1;
volatile uint8_t UARTTxEmpty1 = 1;
volatile uint8_t UARTBuffer1[BUFSIZE];
volatile uint32_t UARTCount1 = 0;

/*****************************************************************************
 ** Function name:		UART_IRQHandler
 **
 ** Descriptions:		UART interrupt handler
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void xUART0_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART0->IIR;

	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) { // Receive Line Status
		LSRValue = LPC_UART0->LSR;
		/* Receive Line Status */
		if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI)) {
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			UARTStatus0 = LSRValue;
			Dummy = LPC_UART0->RBR; /* Dummy read on RX to clear
			 interrupt, then bail out */
			return;
		}
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */
			UARTBuffer0[UARTCount0++] = LPC_UART0->RBR;
			if (UARTCount0 == BUFSIZE) {
				UARTCount0 = 0; /* buffer overflow */
			}
		}
	} else if (IIRValue == IIR_RDA) { 	// Receive Data Available
		/* Receive Data Available */
		UARTBuffer0[UARTCount0++] = LPC_UART0->RBR;
		if (UARTCount0 == BUFSIZE) {
			UARTCount0 = 0; 			// buffer overflow
		}
	} else if (IIRValue == IIR_CTI) { 	// Character timeout indicator
		// Character Time-out indicator
		UARTStatus0 |= 0x100; /* Bit 9 as the CTI error */
	} else if (IIRValue == IIR_THRE) { /* THRE, transmit holding register empty */
		/* THRE interrupt */
		LSRValue = LPC_UART0->LSR; /* Check status in the LSR to see if
		 valid data in U0THR or not */
		if (LSRValue & LSR_THRE) {
			UARTTxEmpty0 = 1;
		} else {
			UARTTxEmpty0 = 0;
		}
	}
	return;
}

/*****************************************************************************
 ** Function name:		UART_IRQHandler
 **
 ** Descriptions:		UART interrupt handler
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void UART1_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART1->IIR;

	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) { /* Receive Line Status */
		LSRValue = LPC_UART1->LSR;
		/* Receive Line Status */
		if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI)) {
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			UARTStatus1 = LSRValue;
			Dummy = LPC_UART1->RBR; /* Dummy read on RX to clear
			 interrupt, then bail out */
			return;
		}
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */
			UARTBuffer1[UARTCount1++] = LPC_UART1->RBR;
			if (UARTCount1 == BUFSIZE) {
				UARTCount1 = 0; /* buffer overflow */
			}
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		/* Receive Data Available */
		UARTBuffer1[UARTCount1++] = LPC_UART1->RBR;
		if (UARTCount1 == BUFSIZE) {
			UARTCount1 = 0; /* buffer overflow */
		}
	} else if (IIRValue == IIR_CTI) /* Character timeout indicator */
	{
		/* Character Time-out indicator */
		UARTStatus1 |= 0x100; /* Bit 9 as the CTI error */
	} else if (IIRValue == IIR_THRE) /* THRE, transmit holding register empty */
	{
		/* THRE interrupt */
		LSRValue = LPC_UART1->LSR; /* Check status in the LSR to see if
		 valid data in U0THR or not */
		if (LSRValue & LSR_THRE) {
			UARTTxEmpty1 = 1;
		} else {
			UARTTxEmpty1 = 0;
		}
	}
	return;
}

/*****************************************************************************
 ** Function name:		SetupUART_Location
 **
 ** Descriptions:		Setup UART0 location
 **
 ** parameters:			port number, location
 ** Returned value:		None
 **
 *****************************************************************************/
void SetupUART_Location(uint32_t portNum, uint32_t location) {
	if (portNum == 0) {
		if (location == 0) {
			LPC_IOCON->PIO0_1 &= ~0x07; /* UART0 I/O config */
			LPC_IOCON->PIO0_1 |= 0x02; /* UART0 RXD LOC0 */
			LPC_IOCON->PIO0_2 &= ~0x07;
			LPC_IOCON->PIO0_2 |= 0x02; /* UART0 TXD LOC0 */
		} else if (location == 1) {
			LPC_IOCON->PIO2_1 &= ~0x07; /* UART0 I/O config */
			LPC_IOCON->PIO2_1 |= 0x04; /* UART0 RXD LOC1 */
			LPC_IOCON->PIO2_2 &= ~0x07;
			LPC_IOCON->PIO2_2 |= 0x04; /* UART0 TXD LOC1 */
		}
	} else if (portNum == 1) {
		if (location == 0) {
			LPC_IOCON->PIO0_8 &= ~0x07; /* UART1 I/O config */
			LPC_IOCON->PIO0_8 |= 0x02; /* UART1 RXD LOC0 */
			LPC_IOCON->PIO0_9 &= ~0x07;
			LPC_IOCON->PIO0_9 |= 0x02; /* UART1 TXD LOC0 */
		} else if (location == 1) {
			LPC_IOCON->PIO2_11 &= ~0x07; /* UART1 I/O config */
			LPC_IOCON->PIO2_11 |= 0x05; /* UART RXD LOC1 */
			LPC_IOCON->PIO2_10 &= ~0x07;
			LPC_IOCON->PIO2_10 |= 0x05; /* UART TXD LOC1 */
		} else {
			LPC_IOCON->PIO2_12 &= ~0x07; /* UART1 I/O config */
			LPC_IOCON->PIO2_12 |= 0x03; /* UART RXD LOC1 */
			LPC_IOCON->PIO2_13 &= ~0x07;
			LPC_IOCON->PIO2_13 |= 0x03; /* UART TXD LOC1 */
		}
	} else {
		while (1)
			;
	}
	return;
}

/*****************************************************************************
 ** Function name:		UARTInit
 **
 ** Descriptions:		Initialize UARTx port, setup pin select,
 **						clock, parity, stop bits, FIFO, etc.
 **
 ** parameters:			UART port number, baudrate
 ** Returned value:		None
 **
 *****************************************************************************/
uint32 UARTInit(uint32_t portNum, uint32_t baudrate) {
	uint32_t i, Fdiv;
	uint32_t regVal;

	if (portNum == 0) {
		UARTTxEmpty0 = 1;
		UARTCount0 = 0;
		for (i = 0; i < BUFSIZE; i++) {
			UARTBuffer0[i] = 0;
		}
		NVIC_DisableIRQ(UART0_IRQn);

//		SetupUART_Location(portNum, 0); /* default is location 0 */

		/* Enable UART 0 clock */
		LPC_SYSCON->PRESETCTRL |= (0x1 << 2);
		LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 12);
		LPC_SYSCON->UART0CLKDIV = 0x1; /* divided by 1 */

		LPC_UART0->LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
		regVal = LPC_SYSCON->UART0CLKDIV;
		Fdiv = ((SystemCoreClock / regVal) / 16) / baudrate; /*baud rate */

		LPC_UART0->DLM = Fdiv / 256;
		LPC_UART0->DLL = Fdiv % 256;
		LPC_UART0->LCR = 0x03; /* DLAB = 0 */
		LPC_UART0->FDR = 0x10; /* set to default value: 0x10 */
		LPC_UART0->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

		/* Read to clear the line status. */
		regVal = LPC_UART0->LSR;

		/* Ensure a clean start, no data in either TX or RX FIFO. */
		while ((LPC_UART0->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT)) ;
		while (LPC_UART0->LSR & LSR_RDR) {
			regVal = LPC_UART0->RBR; /* Dump data from RX FIFO */
		}

		/* Enable the UART Interrupt */
		NVIC_EnableIRQ(UART0_IRQn);

#if TX_INTERRUPT
		LPC_UART0->IER = IER_RBR | IER_THRE | IER_RX; /* Enable UART interrupt */
#else
		LPC_UART0->IER = IER_RBR | IER_RX; /* Enable UART interrupt */
#endif
	} else {
		UARTTxEmpty1 = 1;
		UARTCount1 = 0;
		for (i = 0; i < BUFSIZE; i++) {
			UARTBuffer1[i] = 0;
		}
		NVIC_DisableIRQ(UART1_IRQn);

//		SetupUART_Location(portNum, 0); /* default is location 0 */

		/* Enable UART 1 clock */
		LPC_SYSCON->PRESETCTRL |= (0x1 << 3);
		LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 13);
		LPC_SYSCON->UART1CLKDIV = 0x1; /* divided by 1 */

		LPC_UART1->LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
		regVal = LPC_SYSCON->UART1CLKDIV;
		Fdiv = ((SystemCoreClock / regVal) / 16) / baudrate; /*baud rate */

		LPC_UART1->DLM = Fdiv / 256;
		LPC_UART1->DLL = Fdiv % 256;
		LPC_UART1->LCR = 0x03; /* DLAB = 0 */
		LPC_UART1->FDR = 0x10; /* set to default value: 0x10 */
		LPC_UART1->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

		/* Read to clear the line status. */
		regVal = LPC_UART1->LSR;

		/* Ensure a clean start, no data in either TX or RX FIFO. */
		while ((LPC_UART1->LSR & (LSR_THRE | LSR_TEMT))
				!= (LSR_THRE | LSR_TEMT))
			;
		while (LPC_UART1->LSR & LSR_RDR) {
			regVal = LPC_UART1->RBR; /* Dump data from RX FIFO */
		}

		/* Enable the UART Interrupt */
		NVIC_EnableIRQ(UART1_IRQn);

#if TX_INTERRUPT
		LPC_UART1->IER = IER_RBR | IER_THRE | IER_RX; /* Enable UART interrupt */
#else
		LPC_UART1->IER = IER_RBR | IER_RX; /* Enable UART interrupt */
#endif
	}
	return TRUE;
}

/*****************************************************************************
 ** Function name:		UARTSend
 **
 ** Descriptions:		Send a block of data to the UART 0 port based
 **						on the data length
 **
 ** parameters:			portNum, buffer pointer, and data length
 ** Returned value:		None
 **
 *****************************************************************************/
void UARTSend(uint32_t portNum, uint8_t *BufferPtr, uint32_t Length) {

	while (Length != 0) {
		if (portNum == 0) {
			/* THRE status, contain valid data */
#if !TX_INTERRUPT
			while (!(LPC_UART0->LSR & LSR_THRE))
				;
			LPC_UART0->THR = *BufferPtr;
#else
			/* Below flag is set inside the interrupt handler when THRE occurs. */
			while ( !(UARTTxEmpty0 & 0x01) );
			LPC_UART0->THR = *BufferPtr;
			UARTTxEmpty0 = 0; /* not empty in the THR until it shifts out */
#endif
		} else {
			/* THRE status, contain valid data */
#if !TX_INTERRUPT
			while (!(LPC_UART1->LSR & LSR_THRE))
				;
			LPC_UART1->THR = *BufferPtr;
#else
			/* Below flag is set inside the interrupt handler when THRE occurs. */
			while ( !(UARTTxEmpty1 & 0x01) );
			LPC_UART1->THR = *BufferPtr;
			UARTTxEmpty1 = 0; /* not empty in the THR until it shifts out */
#endif
		}
		BufferPtr++;
		Length--;
	}
	return;
}

/******************************************************************************
 **                            End Of File
 ******************************************************************************/
