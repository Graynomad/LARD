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
//////////////////////////////// LARD.h ////////////////////////////////
//
//
//
//
//

#ifndef LARD_H_
#define LARD_H_

#ifdef __USE_CMSIS
#include "LPC122x.h"
#endif

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <core_cm0.h>


#define 	TRUE	1
#define 	FALSE	0
#define 	ERROR	-1
#define 	NOERROR	0

#define 	HIGH	1
#define 	LOW		0

#define		NULLCHAR '\0'

#define		INLINE		__inline

#define		msTicks		__msTicks

typedef		uint8_t 	boolean;
typedef		uint8_t 	byte;
typedef		uint8_t 	uint8;
typedef		uint16_t 	uint16;
typedef		uint32_t	uint32;
typedef		int32_t		int32;
typedef		int8_t		int8;

#define 	_BV(bit) 	(1 << (bit))

#define     __I     volatile const       // defines 'read only' permissions
#define     __O     volatile             // defines 'write only' permissions
#define     __IO    volatile             // defines 'read / write' permissions

#define 	F_CPU 		35000000UL

#define 	pinPort(pin)(pin / 32)
#define		pinPos(pin)	(pin % 32)

#define 	byte0(w) 	((uint8_t) ((w) & 0xff))
#define 	byte1(w) 	((uint8_t) ((w) >> 8) & 0xFF)
#define 	byte2(w) 	((uint8_t) ((w) >> 16) & 0xFF)
#define 	byte3(w) 	((uint8_t) ((w) >> 24) & 0xFF)

long map(long x, long in_min, long in_max, long out_min, long out_max);


#define	CR_INTEGER_PRINTF	1		// No floating point in Redlib printf()

typedef void 	(*voidFuncPtr)		(void);
typedef uint32 	(*uint32FuncPtr)	(void);

//#include "chip_LPC1227_48PIN.h"
#include "chip_LPC1227_64PIN.h"


#include "interrupts.h"
#include "uart.h"
#include "spi.h"
#include "pin.h"
#include "pingroup.h"
#include "debug.h"
#include "arduino.h"
#include "shift.h"
#include "fifo.h"
#include "hwserial.h"
#include "memory.h"
#include "swtimer.h"
#include "hwtimer.h"
#include "globals.h"
#include "events.h"
#include "adc.h"
#include "debounce.h"
#include "fsm.h"
#include "resources.h"
#include "string.h"
#include "character.h"
#include "vpin.h"
#include "syserror.h"
#include "delay.h"
#include "packet.h"
#include "fatal.h"
#include "app.h"

/////////////////////////////////////
// This typedef is duplicated in fsm.h.
// Duplication required to fix circular forward references
typedef uint32 	(*fsmFuncPtr)		(fsmEvent e);

typedef void 	(*hwTimerFuncPtr)	(hwTimer *);
typedef void 	(*fifoFuncPtr)		(fifo8 *);
//typedef void (*serialFuncPtr)	(serialConnection *, uint32 event);

#if 0

///////////////////////////////////////////////////////////////////////
//
//  Offsets to bits in the AHB clock control register
//
enum {
	AHB_SYS,
	AHB_ROM,
	AHB_RAM,
	AHB_FLASHREG,
	AHB_FLASHARRAY,
	AHB_I2C,
	AHB_CRC,
	AHB_CT16B0,
	AHB_CT16B1,
	AHB_CT32B0,
	AHB_CT32B1,
	AHB_SSP,
	AHB_UART0,
	AHB_UART1,
	AHB_ADC,
	AHB_WDT,
	AHB_IOCON,
	AHB_DMA,
	AHB_NOTUSED,
	AHB_RTC 	= 19,
	AHB_CMP,
	AHB_GPIO2 	= 29,
	AHB_GPIO1,
	AHB_GPIO0

};
#endif

enum {
	OBJID_SPI_CONNECTION = 1,
	OBJID_SERIAL_CONNECTION,
	OBJID_HWTIMER,
	OBJID_SWTIMER,
	OBJID_PINGROUP,
	OBJID_STRING,
	OBJID_FIFO,
	OBJID_ASCII_PACKET,
	OBJID_FSM,
	LAST_LARD_OBJID
};

#endif /* LARD_H_ */
