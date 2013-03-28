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
//////////////////////////////// serial.h ////////////////////////////
//
//
//
//
//

#ifndef SERIAL_H_
#define SERIAL_H_

///////////////////////////////////////////////////////////////////////
//

typedef enum {
	UART_CALLBACK_RX_ERROR,
	UART_CALLBACK_CHARS_AVAILABLE,
	UART_CALLBACK_TX_DONE,
	UART_CALLBACK_RX_CHAR_MATCH,
	UART_CALLBACK_DUMMY
} serialCallback_types;


typedef struct _serialConnection serialConnection;
typedef void (*serialFuncPtr)	(struct _serialConnection *, uint32 event);

struct _serialConnection {

	uint8	object_id;
	uint8	port;
	uint8	data_bits;
	uint8	parity;

	uint8	stop_bits;
	uint8	rxPin;
	uint8	txPin;
	uint8	txState;
	uint8	sending_break;

	fifo8 	* rxBuffer;
	fifo8 	* txBuffer;

	uint32	break_val;

	char	delimBegin;
	char	delimEnd;

	uint32	hwFifoCount;
	uint32	swFifoCount;
	uint32	bitrate;
	uint32	clkDiv;

	serialFuncPtr	callback_funcs[UART_CALLBACK_DUMMY];

	LPC_UART0_Type 	* uart;

	uint8	not_object_id;

} ;

serialConnection * serialCreate (uint8 port, uint32 baudrate, uint8 data_bits, uint8 parity, uint8 stop_bits,
		uint8 rx_buff_size, uint8 tx_buff_size);
uint32 	serialSetUartPins		(serialConnection * s, uint32 location);
void 	serialStartBreak 		(serialConnection * s);
void 	serialEndBreak 			(serialConnection * s);
void 	serialSendBreak 		(serialConnection * s);
uint32 	serialWrite 			(serialConnection * s, uint8 b);
void 	serialPrintString 		(serialConnection * s, char * str);
uint32 	serialRead	 			(serialConnection * s);
uint32 	serialAvailable 		(serialConnection * s);
uint32 	serialSetDataLength 	(serialConnection * s, uint8 data_bits);
uint32 	serialSetStopBits 		(serialConnection * s, uint8 stop_bits);
uint32 	serialSetParity 		(serialConnection * s, uint8 parity);
uint32 	serialSetBitRate 		(serialConnection * s, uint32 rate);
uint32 	serialEnableParity 		(serialConnection * s);
uint32 	serialDisableParity 	(serialConnection * s);
void 	serialFifoWrite 		(serialConnection * s, uint8 b);
uint32 	serialLoopbackOn 		(serialConnection * s);
uint32 	serialLoopbackOff 		(serialConnection * s);
void	serialWaitForTxToClear	(serialConnection * s);
uint32 	serialTxIsIdle			(serialConnection * s);
uint32 	serialTxInProgress		(serialConnection * s);
uint32 	serialGetMaxBufferLevel (serialConnection * s);
void 	serialResetMaxBufferLevel (serialConnection * s);
uint32 	serialAttachCallback 	(serialConnection * s, uint32 event_type, void (*callback_func)(struct _serialConnection * , uint32 e));


enum {
	SERIAL_UART0,	// Allow for up to 6 hardware UARTs
	SERIAL_UART1,
	SERIAL_UART2,
	SERIAL_UART3,
	SERIAL_UART4,
	SERIAL_UART5,
	SERIAL_SOFT0,	// Allow for 2 software serial connections
	SERIAL_SOFT1
};

enum {
	UART_DATA_BITS_5 = 5,
	UART_DATA_BITS_6 = 6,
	UART_DATA_BITS_7 = 7,
	UART_DATA_BITS_8 = 8
};

enum {
	UART_STOP_BITS_1 = (0 << 2),
	UART_STOP_BITS_2 = (1 << 2)
};

enum {
	SERIAL_STATE_IDLE,
	SERIAL_STATE_TRANSMITTING,
	SERIAL_STATE_SENDING_BREAK
};

enum {
	UART_PARITY_NONE	= 0,
	UART_PARITY_ODD		= (1 << 3) | (0 << 4),	// 0x08
	UART_PARITY_EVEN	= (1 << 3) | (1 << 4),	// 0x18
	UART_PARITY_FORCE1	= (1 << 3) | (2 << 4),	// 0x28
	UART_PARITY_FORCE0	= (1 << 3) | (3 << 4),	// 0x38
};

#define MAX_SERIAL_CONNECTIONS	SERIAL_SOFT1

#define	UART0_PINS_R1_T2	0
#define	UART0_PINS_R40_T41	1
#define	UART1_PINS_R8_T9	0
#define	UART1_PINS_R50_T49	1
#define	UART1_PINS_R51_T52	2

#define UART_FIFO_WRITE(s,b)	s->uart->THR = b; \
								s->hwFifoCount++;

#endif /* SERIAL_H_ */
