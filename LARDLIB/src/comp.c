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
///////////////////////////////// comp.c /////////////////////////////////
//
//
//
//
//

#define PullUpDis	  (1<<4) // default enabled
#define AnalogMode	  (1<<7) // enable analog mode

#define	GPIO0_Clock	  (1<<31)
#define Compare_CLK       (1<<20)	// enable clock for comparator module
#define PowerCMP  	 ((1<<3)|(1<<15))	// enable BOD en Comparator module in PDRUNCFG

#define CMP0_EN		 (1)		// enable comparator module 0

#define CMPIEV		 (1<<3)		// activate interrupt on rising edge
#define ACMP0_I0	 (1<<8)		// use ACMP0_O input

#define INTCOMPCLR	 (1<<20)	// bit to reset the interrupt status
#define	CMP0STAT	 (1<<21)	//interrupt status of the comparator 0
#define	CMP1STAT	 (1<<22)	//interrupt status of the comparator 1

#define	VLADEN		 (1)		// enable ladder module ( 32 levels, 0 to 3V3)
#define VSEL		 (0x10<<1)

#define VLADVDDREF	 (1<<6)		// use internal 3V3 voltage to take a level from


#include "LARD.h"

void InitCMP(void){

LPC_SYSCON->SYSAHBCLKCTRL|=Compare_CLK;
LPC_SYSCON->SYSAHBCLKCTRL|=GPIO0_Clock;
LPC_IOCON->PIO0_19|=0x2; // select as input for comparator(ACMP0_O)
LPC_IOCON->PIO0_19&=(~PullUpDis);//disable pullup
LPC_IOCON->PIO0_19&=(~AnalogMode);//enable analog mode
LPC_SYSCON->PDRUNCFG&=(~PowerCMP); // power the comparator module
LPC_ACOMP->VLAD|=(VLADEN|VSEL|VLADVDDREF); // enable voltage ladder, select reference 3.3V, set voltage to 1.66V
LPC_ACOMP->CMP|=(CMP0_EN|CMPIEV|ACMP0_I0|INTCOMPCLR); // enable comparator, use rising edge, positieve input opamp = input ACMP0_I0 , negative = voltage ladder

//NVIC_ClearPendingIRQ(CMP_IRQn);
NVIC_EnableIRQ(CMP_IRQn);
}

void COMP_IRQHandler(void){

LPC_ACOMP->CMP|=INTCOMPCLR;

}
