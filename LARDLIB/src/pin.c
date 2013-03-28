//////////////////////////////// pins.c /////////////////////////////
//
// A set of functions to implement the handling of logical pin on
// the LPC1227 (48-pin version).
//
// Only the 48-pin version of the chip is supported at this time,
// however support for the 64-pin version would be easy to add.
//
// Logical pins the allocated in pin and port order, ie pin0 is
// PO:O, pin20 is P0:20, pin30 is P1:9. There is no provision
// for quasi-random pin allocations as per the Arduino.
//
// TODO: setWriteProtect

#include "LARD.h"

#define 	APIN_2_LPIN(pin) (pin = pin_map[pin])

//////////////////////////////////////////////////////////////////
//
// This array holds the valid functions for each of the 39/55 pins.
//
// Example:		Pin 0 has two functions, GPIO and RTS0 and these
// 				require the values 0 and 2 to be placed in the FUNC
//				field of the pin's IOCONFIG register.
//
// 				The placement of the FUNC_GPIO and FUNC_RTS0 values
//				in this array tells pinFunc()what value to write
//				into the field. Eg FUNC_RTS0 is at location 2 of
//				pin 0's array entry, so the value 2 is written into
//				the lower 3 bits of the IOCONFIG register.
//
const char pin_funcs [55][8] = {
/* 0  */ {FUNC_GPIO,		FUNC_INVALID, 	FUNC_RTS0, 		FUNC_INVALID,		// PIO0_0
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 1  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_RXD0, 		FUNC_CT32B0_CAP0,
		  FUNC_CT32B0_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 2  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_TXD0, 		FUNC_CT32B0_CAP1,
		  FUNC_CT32B0_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 3  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_DTR0, 		FUNC_CT32B0_CAP2,
		  FUNC_CT32B0_MAT2, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 4  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_DSR0, 		FUNC_CT32B0_CAP3,
		  FUNC_CT32B0_MAT3, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 5  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_DCD0, 		FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 6  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_RI0, 		FUNC_CT32B1_CAP0,
		  FUNC_CT32B1_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 7  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CTS0, 		FUNC_CT32B1_CAP1,
		  FUNC_CT32B1_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 8  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_RXD1, 		FUNC_CT32B1_CAP2,
		  FUNC_CT32B1_MAT2, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 9  */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_TXD1, 		FUNC_CT32B1_CAP3,
		  FUNC_CT32B1_MAT3, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 10 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_SCL, 		FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 11 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_SDA, 		FUNC_CT16B0_CAP0,
		  FUNC_CT16B0_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 12 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CLKOUT,  	FUNC_CT16B0_CAP1,
		  FUNC_CT16B0_MAT1, FUNC_INVALID,	FUNC_INVALID, 	FUNC_INVALID},
/* 13 */ {FUNC_RESET, 		FUNC_GPIO,		FUNC_INVALID, 	FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 14 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_SCK, 		FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 15 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_SSEL, 		FUNC_CT16B1_CAP0,
		  FUNC_CT16B1_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 16 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_MISO, 		FUNC_CT16B1_CAP1,
		  FUNC_CT16B1_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 17 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_MOSI, 		FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 18 */ {FUNC_GPIO, 		FUNC_SWCLK, 	FUNC_INVALID, 	FUNC_CT32B0_CAP0,
		  FUNC_CT32B0_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 19 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP0_I0, 	FUNC_CT32B0_CAP1,
		  FUNC_CT32B0_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 20 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP0_I1, 	FUNC_CT32B0_CAP2,
		  FUNC_CT32B0_MAT2, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 21 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP0_I2, 	FUNC_CT32B0_CAP3,
		  FUNC_CT32B0_MAT3, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 22 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP0_I3, 	FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 23 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP1_I0, 	FUNC_CT32B1_CAP0,
		  FUNC_CT32B1_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 24 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP1_I1, 	FUNC_CT32B1_CAP1,
		  FUNC_CT32B1_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 25 */ {FUNC_SWDIO, 		FUNC_INVALID, 	FUNC_ACMP1_I2, 	FUNC_CT32B1_CAP2,
		  FUNC_CT32B1_MAT2, FUNC_INVALID, 	FUNC_GPIO, 		FUNC_INVALID},
/* 26 */ {FUNC_SWCLK, 		FUNC_INVALID, 	FUNC_ACMP1_I3, 	FUNC_CT32B1_CAP3,
		  FUNC_CT32B1_MAT3, FUNC_INVALID, 	FUNC_GPIO, 		FUNC_INVALID},
/* 27 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP0_O, 	FUNC_INVALID,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 28 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ACMP1_O, 	FUNC_CT16B0_CAP0,
		  FUNC_CT16B0_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 29 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ROSC, 		FUNC_CT16B0_CAP1,
		  FUNC_CT16B0_MAT1, FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 30 */ {FUNC_INVALID, 	FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ADC,
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 31 */ {FUNC_INVALID, 	FUNC_GPIO, 		FUNC_INVALID, 	FUNC_ADC,				// PIO0_31
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},

/* 32 */ {FUNC_INVALID, 	FUNC_GPIO, 		FUNC_ADC, 		FUNC_INVALID,			// PIO1_0
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 33 */ {FUNC_INVALID, 	FUNC_GPIO, 		FUNC_ADC, 		FUNC_INVALID,			// PIO1_1
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 34 */ {FUNC_GPIO, 		FUNC_SWDIO, 	FUNC_ADC, 		FUNC_INVALID,			// PIO1_2
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 35 */ {FUNC_GPIO, 		FUNC_ADC, 		FUNC_INVALID, 	FUNC_INVALID,			// PIO1_3
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 36 */ {FUNC_GPIO, 		FUNC_ADC, 		FUNC_INVALID, 	FUNC_INVALID,			// PIO1_4
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 37 */ {FUNC_GPIO, 		FUNC_ADC, 		FUNC_CT16B1_CAP0,FUNC_CT16B1_MAT0,		// PIO1_5
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},
/* 38 */ {FUNC_GPIO, 		FUNC_CT16B1_CAP1, FUNC_CT16B1_MAT1,FUNC_INVALID,		// PIO1_6
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID},

/* 39 */ {FUNC_GPIO,		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_CT16B0_CAP0,		// PIO2_0
		  FUNC_CT16B0_MAT0, FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 40 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT16B0_CAP1,FUNC_CT16B0_MAT1,		// PIO2_1
		  FUNC_RXD0,	 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 41 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT16B1_CAP0,FUNC_CT16B1_MAT0,		// PIO2_2
		  FUNC_TXD0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 42 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT16B1_CAP1,FUNC_CT16B1_MAT1,		// PIO2_3
		  FUNC_DTR0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 43 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B0_CAP0,FUNC_CT32B0_MAT0,		// PIO2_4
		  FUNC_CTS0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 44 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B0_CAP1,FUNC_CT32B0_MAT1,		// PIO2_5
		  FUNC_RI0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 45 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B0_CAP2,FUNC_CT32B0_MAT2,		// PIO2_6
		  FUNC_DCD0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 46 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B0_CAP3,FUNC_CT32B0_MAT3,		// PIO2_7
		  FUNC_DSR0, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 47 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B1_CAP0,FUNC_CT32B1_MAT0,		// PIO2_8
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 48 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B1_CAP1,FUNC_CT32B1_MAT1,		// PIO2_9
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 49 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B1_CAP2,FUNC_CT32B1_MAT2,		// PIO2_10
		  FUNC_INVALID, 	FUNC_TXD1, 		FUNC_INVALID, 	 FUNC_INVALID},
/* 50 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_CT32B1_CAP3,FUNC_CT32B1_MAT3,		// PIO2_11
		  FUNC_INVALID, 	FUNC_RXD1, 		FUNC_INVALID, 	 FUNC_INVALID},
/* 51 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_INVALID,  	 FUNC_RXD1,				// PIO2_12
		  FUNC_INVALID, 	FUNC_INVALID,	FUNC_INVALID, 	 FUNC_INVALID},
/* 52 */ {FUNC_GPIO, 		FUNC_INVALID,	FUNC_INVALID,	 FUNC_TXD1,				// PIO2_13
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 53 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID,			// PIO2_14
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID},
/* 54 */ {FUNC_GPIO, 		FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID,			// PIO2_15
		  FUNC_INVALID, 	FUNC_INVALID, 	FUNC_INVALID, 	 FUNC_INVALID}

};

/////////////////////////////////////////////////////////////////////
//
// Addresses of all 39 (or 55) IOCON registers, one for each pin
//
__IO uint32_t * const pin_regs [] = {
		// Port 0
		&LPC_IOCON->PIO0_0,			// pin 0
		&LPC_IOCON->PIO0_1,			// pin 1
		&LPC_IOCON->PIO0_2,			// pin 2
		&LPC_IOCON->PIO0_3,			// pin 3
		&LPC_IOCON->PIO0_4,			// pin 4
		&LPC_IOCON->PIO0_5,			// pin 5
		&LPC_IOCON->PIO0_6,			// pin 6
		&LPC_IOCON->PIO0_7,			// pin 7
		&LPC_IOCON->PIO0_8,			// pin 8
		&LPC_IOCON->PIO0_9,			// pin 9
		&LPC_IOCON->PIO0_10,		// pin 10
		&LPC_IOCON->PIO0_11,		// pin 11
		&LPC_IOCON->PIO0_12,		// pin 12
		&LPC_IOCON->RESET_PIO0_13,	// pin 13
		&LPC_IOCON->PIO0_14,		// pin 14
		&LPC_IOCON->PIO0_15,		// pin 15
		&LPC_IOCON->PIO0_16,		// pin 16
		&LPC_IOCON->PIO0_17,		// pin 17
		&LPC_IOCON->PIO0_18,		// pin 18
		&LPC_IOCON->PIO0_19,		// pin 19
		&LPC_IOCON->PIO0_20,		// pin 20
		&LPC_IOCON->PIO0_21,		// pin 21
		&LPC_IOCON->PIO0_22,		// pin 22
		&LPC_IOCON->PIO0_23,		// pin 23
		&LPC_IOCON->PIO0_24,		// pin 24
		&LPC_IOCON->SWDIO_PIO0_25,	// pin 25
		&LPC_IOCON->SWCLK_PIO0_26,	// pin 26
		&LPC_IOCON->PIO0_27,		// pin 27
		&LPC_IOCON->PIO0_28,		// pin 28
		&LPC_IOCON->PIO0_29,		// pin 29
		&LPC_IOCON->R_PIO0_30,		// pin 30
		&LPC_IOCON->R_PIO0_31,		// pin 31

		// Port 1
		&LPC_IOCON->R_PIO1_0,		// pin 32
		&LPC_IOCON->R_PIO1_1,		// pin 33
		&LPC_IOCON->PIO1_2,			// pin 34
		&LPC_IOCON->PIO1_3,			// pin 35
		&LPC_IOCON->PIO1_4,			// pin 36
		&LPC_IOCON->PIO1_5,			// pin 37
		&LPC_IOCON->PIO1_6,			// pin 38

		// Port 2
		&LPC_IOCON->PIO2_0,			// pin 39
		&LPC_IOCON->PIO2_1,			// pin 40
		&LPC_IOCON->PIO2_2,			// pin 41
		&LPC_IOCON->PIO2_3,			// pin 42
		&LPC_IOCON->PIO1_4,			// pin 43
		&LPC_IOCON->PIO2_5,			// pin 44
		&LPC_IOCON->PIO2_6,			// pin 45
		&LPC_IOCON->PIO2_7,			// pin 46
		&LPC_IOCON->PIO2_8,			// pin 47
		&LPC_IOCON->PIO2_9,			// pin 48
		&LPC_IOCON->PIO2_10,		// pin 49
		&LPC_IOCON->PIO2_11,		// pin 50
		&LPC_IOCON->PIO2_12,		// pin 51
		&LPC_IOCON->PIO2_13,		// pin 52
		&LPC_IOCON->PIO2_14,		// pin 53
		&LPC_IOCON->PIO2_15			// pin 54
};

/////////////////////////////////////////////////////////////////////
//
// Array of pointers to the 14 special GPIO registers.
//
// Most of the regs are read/write but PIN, RIS and MIS are read only
// and defined as __I so they have been cast to __IO to keep the
// compiler quite.
//
__IO uint32_t * const gpio_regs [14][3] = {
		{&LPC_GPIO0->MASK,	&LPC_GPIO1->MASK,	&LPC_GPIO2->MASK},

		{(__IO uint32_t *)	&LPC_GPIO0->PIN,
		 (__IO uint32_t *)	&LPC_GPIO1->PIN,
		 (__IO uint32_t *)	&LPC_GPIO2->PIN},

		{&LPC_GPIO0->OUT,	&LPC_GPIO1->OUT,	&LPC_GPIO2->OUT},
		{&LPC_GPIO0->SET,	&LPC_GPIO1->SET,	&LPC_GPIO2->SET},
		{&LPC_GPIO0->CLR,	&LPC_GPIO1->CLR,	&LPC_GPIO2->CLR},
		{&LPC_GPIO0->NOT,	&LPC_GPIO1->NOT,	&LPC_GPIO2->NOT},
		{&LPC_GPIO0->DIR,	&LPC_GPIO1->DIR,	&LPC_GPIO2->DIR},
		{&LPC_GPIO0->IS, 	&LPC_GPIO1->IS,		&LPC_GPIO2->IS},
		{&LPC_GPIO0->IBE,	&LPC_GPIO1->IBE,	&LPC_GPIO2->IBE},
		{&LPC_GPIO0->IEV,	&LPC_GPIO1->IEV,	&LPC_GPIO2->IEV},
		{&LPC_GPIO0->IE, 	&LPC_GPIO1->IE,		&LPC_GPIO2->IE},

		{(__IO uint32_t *)	&LPC_GPIO0->RIS,
		 (__IO uint32_t *)	&LPC_GPIO1->RIS,
		 (__IO uint32_t *)	&LPC_GPIO2->RIS},

		{(__IO uint32_t *)	&LPC_GPIO0->MIS,
		 (__IO uint32_t *)	&LPC_GPIO1->MIS,
		 (__IO uint32_t *)	&LPC_GPIO2->MIS},

		{&LPC_GPIO0->IC,	&LPC_GPIO1->IC,		&LPC_GPIO2->IC}
};

uint32	nativeDpins(void) {
	return NATIVE_PINS;
}

/////////////////////////////////////////////////////////////////////
//
// Non-user function to set any GPIO register bit
//
uint32 _pinSetGpioBit(uint32 pin, uint32 gpio_reg) {

	APIN_2_LPIN (pin);		// map application pin to logical pin

	if (pin >= NATIVE_PINS) {
		SYS_ERROR (ERR_INV_PIN);
		return ERROR;
	}

	uint32 port;

	switch (gpio_reg) {

		case GPIOREG_PIN:	// Read only registers
		case GPIOREG_MIS:
		case GPIOREG_RIS:
			return ERROR;

		default:
			port = pinPort(pin);
			*gpio_regs[gpio_reg][port] |= word_bits[pin - (32 * port)];
			return NOERROR;
	}
}

////////////////////////////////////////////////////////////////////
//
// Non-exported function to clear any GPIO register bit
//
boolean _pinClearGpioBit(uint32 pin, uint32 gpio_reg) {

	APIN_2_LPIN (pin);		// map application pin to logical pin

	if (pin >= NATIVE_PINS) {
		SYS_ERROR (ERR_INV_PIN);
		return ERROR;
	}

	uint32 port;

	switch (gpio_reg) {

		case GPIOREG_PIN:	// Read only registers
		case GPIOREG_MIS:
		case GPIOREG_RIS:
			return ERROR;

		default:
			port = pinPort(pin);
			*gpio_regs[gpio_reg][port] &= ~(word_bits[pin - (32 * port)]);
			return NOERROR;
	}
}

#ifdef xLPC1227_64PIN
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
#endif

#ifdef XLPC1227_48PIN
uint8	const __physical_pins [] = {
		// 	0	1	2	3	4	5	6	7	8	9      logical pin #
			15,	16,	17,	18,	19,	20,	21,	22,	23,	24, // physical pin #

		// 	10	11	12	13	14	15	16	17	18	19
			25,	26,	27,	28,	29,	30,	31,	32,	33,	4,

		// 	20	21	22	23	24	25	26	27	28	29
			5,	6,	7,	8,	9,	10,	11,	12,	13,	14,

		// 	30	31	32	33	34	35	36	37	38	39
			34,	35,	36,	37,	38,	39,	40,	41,	42
};
#endif

uint32 	pinGetPhyiscalPin (uint32 pin) {

	ASSERT_RETERR (pin > NATIVE_PINS, ERR_INV_PIN | pin);

	return __physical_pins[pin];

}

/////////////////////////////////////////////////////////////////////
//
// Function name:		pinXxx
//
// Description:			A series of functions to set, clear, mask etc a pin.
//
// Parameters:			uint32 pin, the logical pin number: valid values 0-38/54
//
// Returned value:		NOERROR if no errors, ERROR if there are.
//						Except pinRead which returns the logic state of the pin
///
// Errors raised:		ERR_INV_PIN if the pin number was invalid.
//
//
uint32 pinHigh(uint32 pin) {

	return _pinSetGpioBit (pin, GPIOREG_SET);

}

uint32 pinLow(uint32 pin) {

	return _pinSetGpioBit (pin, GPIOREG_CLR);

}

uint32 pinWrite(uint32 pin, uint32 val) {
	// NOTE: Any non-zero value sets the pin
	if (val)
		return _pinSetGpioBit (pin, GPIOREG_SET);
	else
		return _pinSetGpioBit (pin, GPIOREG_CLR);

}

uint32 pinRead(uint32 pin) {

	uint32 val, port;

	port = pinPort(pin);
	val = *gpio_regs[GPIOREG_PIN][port];	// Get all 32 bits from the port
	val >>= (pin % 32);						// Shift our bit to the LS position
	val &= 1;								// Ditch any other bits

	return val;

}

uint32 pinToggle(uint32 pin) {

	return _pinSetGpioBit (pin, GPIOREG_NOT);

}

INLINE uint32 pinPulse(uint32 pin) {

	_pinSetGpioBit (pin, GPIOREG_NOT);
	return _pinSetGpioBit (pin, GPIOREG_NOT);

}


uint32 pinMask(uint32 pin) {

	return _pinSetGpioBit (pin, GPIOREG_MASK);

}

uint32 pinSetAsDigitalInput(uint32 pin) {

	uint32 retval = 0;

	retval |= _pinClearGpioBit (pin, GPIOREG_DIR);
	retval |= pinFunc (pin, FUNC_GPIO);
	retval |= pinConfig (pin, PIN_ADMODE_DISABLED);

	return (retval);

}

uint32 pinSetAsDigitalOutput(uint32 pin) {

	uint32 retval = 0;

	retval |= _pinSetGpioBit (pin, GPIOREG_DIR);
	retval |= pinFunc (pin, FUNC_GPIO);
	retval |= pinConfig (pin, PIN_ADMODE_DISABLED);

	return (retval);

}

uint32 pinSetAsAnalogInput(uint32 pin) {

	uint32 retval = 0;

	retval |= _pinClearGpioBit (pin, GPIOREG_DIR);
	retval |= pinFunc (pin, FUNC_ADC);
	retval |= pinConfig (pin, PIN_ADMODE_ENABLED | PIN_MODE_NOPULLUP);

	return (retval);

}

uint32 pinSetInterruptMode (uint32 pin, uint8 mode) {

	uint32 retval = 0;

	retval |= _pinClearGpioBit (pin, GPIOREG_IS);
	retval |= _pinClearGpioBit (pin, GPIOREG_IEV);
	retval |= _pinClearGpioBit (pin, GPIOREG_IBE);

	switch (mode) {
		case PININT_HIGH_LEVEL:
			retval |= _pinSetGpioBit (pin, GPIOREG_IS);
			retval |= _pinSetGpioBit (pin, GPIOREG_IEV);
			break;

		case PININT_LOW_LEVEL:
			retval |= _pinSetGpioBit (pin, GPIOREG_IS);
			break;

		case PININT_RISING_EDGE:
			retval |= _pinSetGpioBit (pin, GPIOREG_IEV);
			break;

		case PININT_FALLING_EDGE:
			break;

		case PININT_BOTH_EDGES:
			retval |= _pinSetGpioBit (pin, GPIOREG_IBE);
			break;
	}

	return (retval);

}

uint32 pinEnableInterrupt (uint32 pin) {

	uint32 retval = 0;

	retval |= _pinSetGpioBit (pin, GPIOREG_IC); // clear any current interrupt
	retval |= _pinSetGpioBit (pin, GPIOREG_IE); // then enable

	return (retval);

}

uint32 pinDisableInterrupt (uint32 pin) {

	uint32 retval = 0;

	retval |= _pinClearGpioBit (pin, GPIOREG_IE); // disable interrupt
	retval |= _pinSetGpioBit (pin, GPIOREG_IC);   // clear any pending interrupts

	return (retval);

}

uint32 pinClearInterrupt (uint32 pin) {

	return _pinSetGpioBit (pin, GPIOREG_IC);

}

/////////////////////////////////////////////////////////////////////
//
// Function name:		pinConfig
//
// Description:			Set the mode of a pin, the "mode" determine features such
//						as pullup resistors, polarity, open drain etc.
//
// Parameters:			uint32 pin: The logical pin number, valid values are 0-38/54
//						uint32 mode: A bit pattern appropriate for setting the
//							bits in the IOCONFIG register, NOT including the FUNC
//							field.
//
// Returned value:		ERROR if an error was encountered, NOERROR if not.
//
// Errors raised:		ERR_INV_PIN if the pin number was invalid.
//
// Notes:				The bit pattern is constructed from the defined values in
//						pins.h. These values are OR'd together to make a single
//						value that is placed in the upper 29 bits of the pin's
//						IOCONFIG register
//
//						The three Arduino pinMode constants are trapped and converted
//						to appropriate formats.
//
// Example:				pinConfig (1, PIN_OD_ENABLED | PIN_INV_YES);
//						Set pin to be open drain and inverted.
//
uint32 pinConfig(uint32 pin, uint32 mode) {

	if (pin >= NATIVE_PINS) {
		SYS_ERROR (ERR_INV_PIN | pin);
		return ERROR;
	}

	////////////////////////////////////////////////////////////
	//
	// Handle the three standard Arduino arguments for pinMode
	//
	// Note that the calls to pinSetAsDigitalInput/Output cause
	// a recursive call to this function.
	//
	switch (mode) {

		case INPUT:
			pinSetAsDigitalInput(pin);
			return NOERROR;

		case OUTPUT:
			pinSetAsDigitalOutput(pin);
			return NOERROR;

		case INPUT_PULLUP:
			pinSetAsDigitalInput(pin);
			mode = PIN_MODE_PULLUP | PIN_ADMODE_DISABLED;
			break;
	}

	////////////////////////////////////////////////////////////
	//
	// Set appropriate bits in the pin's IOCON register
	//
	*pin_regs[pin] &= 3;  	// clear all but the FUNC field
	*pin_regs[pin] |= mode; // write the mode bits

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		pinFunc
//
// Description:			Set the function of a pin. The function sets the pin's MUX
//						and selects one of the possible functions the pin can perform,
//						eg GPIO, UART RX, TIMER IO. These are different for every pin.
//
// Parameters:			uint32 pin: The logical pin number, valid values are 0-38
//						uint32 func: A value from the gpio_func_types enum.
//
// Returned value:		NOERROR if no errors
//						ERROR if there are errors, eg you asked for a function the
//							is not implemented for the pin in question.
//
// Errors raised:		ERR_INV_PIN if the pin number was invalid.
//						(ERR_BAD_PIN_FUNC | func) if the function requested is not valid
//							for the pin.
//
// Notes:				The func field is the lowest 3 bits of the pin's IOCONFIG
//						register. This function scans the 8 values in the pins entry
//						in the pin_funcs array looking for a match with the supplied
//						function. If one is found it's index value is written
//						into the lower 3 bits. If no match is found no action is taken.
//
// Example:				pinFunc (1, FUNC_CT32B0_MAT0);
//						Set pin 1's function to the match output, channel 0 for 32-bit timer 0.
//
uint32 pinFunc(uint32 pin, uint32 func) {

	if (pin >= NATIVE_PINS) {
		ASSERT (pin >= NATIVE_PINS, ERR_INV_PIN | pin);
		SYS_ERROR (ERR_INV_PIN | pin);
		return ERROR;
	}

	for (int i = 0; i < 8; i++) {
		if (pin_funcs[pin][i] == func) {
			*pin_regs[pin] &= ~3;  		// clear the func field
			*pin_regs[pin] |= i;		// write into the func field
			resourceRecordUsage (pin);	// record that this pin has been configured
			return NOERROR;
		}
	}

	SYS_ERROR (ERR_BAD_PIN_FUNC | func);
	return ERROR;
}

