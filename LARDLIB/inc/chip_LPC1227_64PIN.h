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
/////////////////////////////// chip_LPC1227_64PIN.h ///////////////////////
//
// Use this file for any chip-specific constants and tables
// There is probably still stuff in the main files that needs to be moved here
//


#ifndef CHIP_LPC1227_48_H_
#define CHIP_LPC1227_48_H_

#define LPC1227_64PIN

#define	NATIVE_PINS	55
#define	N_UARTS		2


enum {
	PINS_ON_PORT0	= 32,
	PINS_ON_PORT1	= 7,
	PINS_ON_PORT2	= 16
};


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

uint8	const __physical_pins [] = {
		// 	0	1	2	3	4	5	6	7	8	9      logical pin #
			19,	20,	21,	22,	23,	24,	25,	26,	27,	28, // physical pin #

		// 	10	11	12	13	14	15	16	17	18	19
			37,	38,	39,	40,	41,	42,	43,	44,	45,	4,

		// 	20	21	22	23	24	25	26	27	28	29
			5,	6,	7,	8,	9,	10,	11,	12,	17,	18,

		// 	30	31	32	33	34	35	36	37	38	39
			46,	47,	48,	49,	50,	51,	53,	54,	42,	29,

		// 	40	41	42	43	44	45	46	47	48	49
			30,	31,	32,	33,	34,	35,	36,	59,	60,	61,

		// 	50	51	52	53	54
			62,	13,	14,	15,	16
};

////////////////////////////////////////////////////////////////
//
// Macros for fast pin manipulation
//
// Note that the form
//
// LPC_GPIO0->CLR = (1 << 7)
//
// is used, it should be faster to use
//
// (*(uint32*)0x50000014 = (1 << 7))
//
// and in fact this form compiles to fewer instructions, however
// testing shows that the two forms execute in the same time
// so the more portable and readable version is used.
//


#define pin0toggle()	(LPC_GPIO0->NOT = (1 << 0))
#define pin1toggle()	(LPC_GPIO0->NOT = (1 << 1))
#define pin2toggle()	(LPC_GPIO0->NOT = (1 << 2))
#define pin3toggle()	(LPC_GPIO0->NOT = (1 << 3))
#define pin4toggle()	(LPC_GPIO0->NOT = (1 << 4))
#define pin5toggle()	(LPC_GPIO0->NOT = (1 << 5))
#define pin6toggle()	(LPC_GPIO0->NOT = (1 << 6))
#define pin7toggle()	(LPC_GPIO0->NOT = (1 << 7))
#define pin8toggle()	(LPC_GPIO0->NOT = (1 << 8))
#define pin9toggle()	(LPC_GPIO0->NOT = (1 << 9))
#define pin10toggle()	(LPC_GPIO0->NOT = (1 << 10))
#define pin11toggle()	(LPC_GPIO0->NOT = (1 << 11))
#define pin12toggle()	(LPC_GPIO0->NOT = (1 << 12))
#define pin13toggle()	(LPC_GPIO0->NOT = (1 << 13))
#define pin14toggle()	(LPC_GPIO0->NOT = (1 << 14))
#define pin15toggle()	(LPC_GPIO0->NOT = (1 << 15))
#define pin16toggle()	(LPC_GPIO0->NOT = (1 << 16))
#define pin17toggle()	(LPC_GPIO0->NOT = (1 << 17))
#define pin18toggle()	(LPC_GPIO0->NOT = (1 << 18))
#define pin19toggle()	(LPC_GPIO0->NOT = (1 << 19))
#define pin20toggle()	(LPC_GPIO0->NOT = (1 << 20))
#define pin21toggle()	(LPC_GPIO0->NOT = (1 << 21))
#define pin22toggle()	(LPC_GPIO0->NOT = (1 << 22))
#define pin23toggle()	(LPC_GPIO0->NOT = (1 << 23))
#define pin24toggle()	(LPC_GPIO0->NOT = (1 << 24))
#define pin25toggle()	(LPC_GPIO0->NOT = (1 << 25))
#define pin26toggle()	(LPC_GPIO0->NOT = (1 << 26))
#define pin27toggle()	(LPC_GPIO0->NOT = (1 << 27))
#define pin28toggle()	(LPC_GPIO0->NOT = (1 << 28))
#define pin29toggle()	(LPC_GPIO0->NOT = (1 << 29))
#define pin30toggle()	(LPC_GPIO0->NOT = (1 << 30))
#define pin31toggle()	(LPC_GPIO0->NOT = (1 << 31))
#define pin32toggle()	(LPC_GPIO1->NOT = (1 << 0))
#define pin33toggle()	(LPC_GPIO1->NOT = (1 << 1))
#define pin34toggle()	(LPC_GPIO1->NOT = (1 << 2))
#define pin35toggle()	(LPC_GPIO1->NOT = (1 << 3))
#define pin36toggle()	(LPC_GPIO1->NOT = (1 << 4))
#define pin37toggle()	(LPC_GPIO1->NOT = (1 << 5))
#define pin38toggle()	(LPC_GPIO1->NOT = (1 << 6))
#define pin39toggle()	(LPC_GPIO2->NOT = (1 << 0))
#define pin40toggle()	(LPC_GPIO2->NOT = (1 << 1))
#define pin41toggle()	(LPC_GPIO2->NOT = (1 << 2))
#define pin42toggle()	(LPC_GPIO2->NOT = (1 << 3))
#define pin43toggle()	(LPC_GPIO2->NOT = (1 << 4))
#define pin44toggle()	(LPC_GPIO2->NOT = (1 << 5))
#define pin45toggle()	(LPC_GPIO2->NOT = (1 << 6))
#define pin46toggle()	(LPC_GPIO2->NOT = (1 << 7))
#define pin47toggle()	(LPC_GPIO2->NOT = (1 << 8))
#define pin48toggle()	(LPC_GPIO2->NOT = (1 << 9))
#define pin49toggle()	(LPC_GPIO2->NOT = (1 << 10))
#define pin50toggle()	(LPC_GPIO2->NOT = (1 << 11))
#define pin51toggle()	(LPC_GPIO2->NOT = (1 << 12))
#define pin52toggle()	(LPC_GPIO2->NOT = (1 << 13))
#define pin53toggle()	(LPC_GPIO2->NOT = (1 << 14))
#define pin54toggle()	(LPC_GPIO2->NOT = (1 << 15))

#define pin0low()		(LPC_GPIO0->CLR = (1 << 0))
#define pin1low()		(LPC_GPIO0->CLR = (1 << 1))
#define pin2low()		(LPC_GPIO0->CLR = (1 << 2))
#define pin3low()		(LPC_GPIO0->CLR = (1 << 3))
#define pin4low()		(LPC_GPIO0->CLR = (1 << 4))
#define pin5low()		(LPC_GPIO0->CLR = (1 << 5))
#define pin6low()		(LPC_GPIO0->CLR = (1 << 6))
#define pin7low()		(LPC_GPIO0->CLR = (1 << 7))
#define pin8low()		(LPC_GPIO0->CLR = (1 << 8))
#define pin9low()		(LPC_GPIO0->CLR = (1 << 9))
#define pin10low()		(LPC_GPIO0->CLR = (1 << 10))
#define pin11low()		(LPC_GPIO0->CLR = (1 << 11))
#define pin12low()		(LPC_GPIO0->CLR = (1 << 12))
#define pin13low()		(LPC_GPIO0->CLR = (1 << 13))
#define pin14low()		(LPC_GPIO0->CLR = (1 << 14))
#define pin15low()		(LPC_GPIO0->CLR = (1 << 15))
#define pin16low()		(LPC_GPIO0->CLR = (1 << 16))
#define pin17low()		(LPC_GPIO0->CLR = (1 << 17))
#define pin18low()		(LPC_GPIO0->CLR = (1 << 18))
#define pin19low()		(LPC_GPIO0->CLR = (1 << 19))
#define pin20low()		(LPC_GPIO0->CLR = (1 << 20))
#define pin21low()		(LPC_GPIO0->CLR = (1 << 21))
#define pin22low()		(LPC_GPIO0->CLR = (1 << 22))
#define pin23low()		(LPC_GPIO0->CLR = (1 << 23))
#define pin24low()		(LPC_GPIO0->CLR = (1 << 24))
#define pin25low()		(LPC_GPIO0->CLR = (1 << 25))
#define pin26low()		(LPC_GPIO0->CLR = (1 << 26))
#define pin27low()		(LPC_GPIO0->CLR = (1 << 27))
#define pin28low()		(LPC_GPIO0->CLR = (1 << 28))
#define pin29low()		(LPC_GPIO0->CLR = (1 << 29))
#define pin30low()		(LPC_GPIO0->CLR = (1 << 30))
#define pin31low()		(LPC_GPIO0->CLR = (1 << 31))
#define pin32low()		(LPC_GPIO1->CLR = (1 << 0))
#define pin33low()		(LPC_GPIO1->CLR = (1 << 1))
#define pin34low()		(LPC_GPIO1->CLR = (1 << 2))
#define pin35low()		(LPC_GPIO1->CLR = (1 << 3))
#define pin36low()		(LPC_GPIO1->CLR = (1 << 4))
#define pin37low()		(LPC_GPIO1->CLR = (1 << 5))
#define pin38low()		(LPC_GPIO1->CLR = (1 << 6))
#define pin39low()		(LPC_GPIO2->CLR = (1 << 0))
#define pin40low()		(LPC_GPIO2->CLR = (1 << 1))
#define pin41low()		(LPC_GPIO2->CLR = (1 << 2))
#define pin42low()		(LPC_GPIO2->CLR = (1 << 3))
#define pin43low()		(LPC_GPIO2->CLR = (1 << 4))
#define pin44low()		(LPC_GPIO2->CLR = (1 << 5))
#define pin45low()		(LPC_GPIO2->CLR = (1 << 6))
#define pin46low()		(LPC_GPIO2->CLR = (1 << 7))
#define pin47low()		(LPC_GPIO2->CLR = (1 << 8))
#define pin48low()		(LPC_GPIO2->CLR = (1 << 9))
#define pin49low()		(LPC_GPIO2->CLR = (1 << 10))
#define pin50low()		(LPC_GPIO2->CLR = (1 << 11))
#define pin51low()		(LPC_GPIO2->CLR = (1 << 12))
#define pin52low()		(LPC_GPIO2->CLR = (1 << 13))
#define pin53low()		(LPC_GPIO2->CLR = (1 << 14))
#define pin54low()		(LPC_GPIO2->CLR = (1 << 15))

#define pin0high()		(LPC_GPIO0->SET = (1 << 0))
#define pin1high()		(LPC_GPIO0->SET = (1 << 1))
#define pin2high()		(LPC_GPIO0->SET = (1 << 2))
#define pin3high()		(LPC_GPIO0->SET = (1 << 3))
#define pin4high()		(LPC_GPIO0->SET = (1 << 4))
#define pin5high()		(LPC_GPIO0->SET = (1 << 5))
#define pin6high()		(LPC_GPIO0->SET = (1 << 6))
#define pin7high()		(LPC_GPIO0->SET = (1 << 7))
#define pin8high()		(LPC_GPIO0->SET = (1 << 8))
#define pin9high()		(LPC_GPIO0->SET = (1 << 9))
#define pin10high()		(LPC_GPIO0->SET = (1 << 10))
#define pin11high()		(LPC_GPIO0->SET = (1 << 11))
#define pin12high()		(LPC_GPIO0->SET = (1 << 12))
#define pin13high()		(LPC_GPIO0->SET = (1 << 13))
#define pin14high()		(LPC_GPIO0->SET = (1 << 14))
#define pin15high()		(LPC_GPIO0->SET = (1 << 15))
#define pin16high()		(LPC_GPIO0->SET = (1 << 16))
#define pin17high()		(LPC_GPIO0->SET = (1 << 17))
#define pin18high()		(LPC_GPIO0->SET = (1 << 18))
#define pin19high()		(LPC_GPIO0->SET = (1 << 19))
#define pin20high()		(LPC_GPIO0->SET = (1 << 20))
#define pin21high()		(LPC_GPIO0->SET = (1 << 21))
#define pin22high()		(LPC_GPIO0->SET = (1 << 22))
#define pin23high()		(LPC_GPIO0->SET = (1 << 23))
#define pin24high()		(LPC_GPIO0->SET = (1 << 24))
#define pin25high()		(LPC_GPIO0->SET = (1 << 25))
#define pin26high()		(LPC_GPIO0->SET = (1 << 26))
#define pin27high()		(LPC_GPIO0->SET = (1 << 27))
#define pin28high()		(LPC_GPIO0->SET = (1 << 28))
#define pin29high()		(LPC_GPIO0->SET = (1 << 29))
#define pin30high()		(LPC_GPIO0->SET = (1 << 30))
#define pin31high()		(LPC_GPIO0->SET = (1 << 31))
#define pin32high()		(LPC_GPIO1->SET = (1 << 0))
#define pin33high()		(LPC_GPIO1->SET = (1 << 1))
#define pin34high()		(LPC_GPIO1->SET = (1 << 2))
#define pin35high()		(LPC_GPIO1->SET = (1 << 3))
#define pin36high()		(LPC_GPIO1->SET = (1 << 4))
#define pin37high()		(LPC_GPIO1->SET = (1 << 5))
#define pin38high()		(LPC_GPIO1->SET = (1 << 6))
#define pin39high()		(LPC_GPIO2->SET = (1 << 0))
#define pin40high()		(LPC_GPIO2->SET = (1 << 1))
#define pin41high()		(LPC_GPIO2->SET = (1 << 2))
#define pin42high()		(LPC_GPIO2->SET = (1 << 3))
#define pin43high()		(LPC_GPIO2->SET = (1 << 4))
#define pin44high()		(LPC_GPIO2->SET = (1 << 5))
#define pin45high()		(LPC_GPIO2->SET = (1 << 6))
#define pin46high()		(LPC_GPIO2->SET = (1 << 7))
#define pin47high()		(LPC_GPIO2->SET = (1 << 8))
#define pin48high()		(LPC_GPIO2->SET = (1 << 9))
#define pin49high()		(LPC_GPIO2->SET = (1 << 10))
#define pin50high()		(LPC_GPIO2->SET = (1 << 11))
#define pin51high()		(LPC_GPIO2->SET = (1 << 12))
#define pin52high()		(LPC_GPIO2->SET = (1 << 13))
#define pin53high()		(LPC_GPIO2->SET = (1 << 14))
#define pin54high()		(LPC_GPIO2->SET = (1 << 15))

#define pin0pulse()			pin0toggle();pin0toggle();
#define pin1pulse()			pin1toggle();pin1toggle();
#define pin2pulse()			pin2toggle();pin2toggle();
#define pin3pulse()			pin3toggle();pin3toggle();
#define pin4pulse()			pin4toggle();pin4toggle();
#define pin5pulse()			pin5toggle();pin5toggle();
#define pin6pulse()			pin6toggle();pin6toggle();
#define pin7pulse()			pin7toggle();pin7toggle();
#define pin8pulse()			pin8toggle();pin8toggle();
#define pin9pulse()			pin9toggle();pin9toggle();
#define pin10pulse()		pin10toggle();pin10toggle();
#define pin11pulse()		pin11toggle();pin11toggle();
#define pin12pulse()		pin12toggle();pin12toggle();
#define pin13pulse()		pin13toggle();pin13toggle();
#define pin14pulse()		pin14toggle();pin14toggle();
#define pin15pulse()		pin15toggle();pin15toggle();
#define pin16pulse()		pin16toggle();pin16toggle();
#define pin17pulse()		pin17toggle();pin17toggle();
#define pin18pulse()		pin18toggle();pin18toggle();
#define pin19pulse()		pin19toggle();pin19toggle();
#define pin20pulse()		pin20toggle();pin20toggle();
#define pin21pulse()		pin21toggle();pin21toggle();
#define pin22pulse()		pin22toggle();pin22toggle();
#define pin23pulse()		pin23toggle();pin23toggle();
#define pin24pulse()		pin24toggle();pin24toggle();
#define pin25pulse()		pin25toggle();pin25toggle();
#define pin26pulse()		pin26toggle();pin26toggle();
#define pin27pulse()		pin27toggle();pin27toggle();
#define pin28pulse()		pin28toggle();pin28toggle();
#define pin29pulse()		pin29toggle();pin29toggle();
#define pin30pulse()		pin30toggle();pin30toggle();
#define pin31pulse()		pin31toggle();pin31toggle();
#define pin32pulse()		pin32toggle();pin32toggle();
#define pin33pulse()		pin33toggle();pin33toggle();
#define pin34pulse()		pin34toggle();pin34toggle();
#define pin35pulse()		pin35toggle();pin35toggle();
#define pin36pulse()		pin36toggle();pin36toggle();
#define pin37pulse()		pin37toggle();pin37toggle();
#define pin38pulse()		pin38toggle();pin38toggle();
#define pin39pulse()		pin39toggle();pin39toggle();
#define pin40pulse()		pin40toggle();pin40toggle();
#define pin41pulse()		pin41toggle();pin41toggle();
#define pin42pulse()		pin42toggle();pin42toggle();
#define pin43pulse()		pin43toggle();pin43toggle();
#define pin44pulse()		pin44toggle();pin44toggle();
#define pin45pulse()		pin45toggle();pin45toggle();
#define pin46pulse()		pin46toggle();pin46toggle();
#define pin47pulse()		pin47toggle();pin47toggle();
#define pin48pulse()		pin48toggle();pin48toggle();
#define pin49pulse()		pin49toggle();pin49toggle();
#define pin50pulse()		pin50toggle();pin50toggle();
#define pin51pulse()		pin51toggle();pin51toggle();
#define pin52pulse()		pin52toggle();pin52toggle();
#define pin53pulse()		pin53toggle();pin53toggle();
#define pin54pulse()		pin54toggle();pin54toggle();

#endif /* CHIP_LPC1227_48_H_ */
