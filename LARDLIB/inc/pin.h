//////////////////////////////// pin.h ////////////////////////////
//
//
//
//
//

#ifndef PIN_H_
#define PIN_H_

////////////////////////////////////////////////////////////////
//
// Function prototypes.
//
uint32	pinConfig				(uint32 pin, uint32 mode);
uint32	pinFunc					(uint32 pin, uint32 func);
uint32	pinLow					(uint32 pin);
uint32	pinHigh					(uint32 pin);
uint32  pinRead					(uint32 pin);
uint32	pinWrite				(uint32 pin, uint32 val);
uint32	pinMask					(uint32 pin);
uint32	pinToggle				(uint32 pin);
uint32	pinPulse				(uint32 pin);
uint32	pinSetAsDigitalInput	(uint32 pin);
uint32	pinSetAsDigitalOutput	(uint32 pin);
uint32	pinSetAsAnalogInput		(uint32 pin);
uint32	nativeDpins				(void);
uint32 	pinSetInterruptMode 	(uint32 pin, uint8 mode);
uint32 	pinEnableInterrupt 		(uint32 pin);
uint32 	pinDisableInterrupt 	(uint32 pin) ;
uint32 	pinClearInterrupt 		(uint32 pin);
uint32 	pinGetPhyiscalPin 		(uint32 pin);

////////////////////////////////////////////////////////////////
//
// Values passed to the pinFunc() function
//
typedef enum  {
	FUNC_INVALID,

	FUNC_GPIO,

	FUNC_ACMP0_O,
	FUNC_ACMP0_I0,
	FUNC_ACMP0_I1,
	FUNC_ACMP0_I2,
	FUNC_ACMP0_I3,

	FUNC_ACMP1_O,
	FUNC_ACMP1_I0,
	FUNC_ACMP1_I1,
	FUNC_ACMP1_I2,
	FUNC_ACMP1_I3,

	FUNC_RXD1,
	FUNC_TXD1,
	FUNC_TXD0,
	FUNC_RXD0,

	FUNC_RTS0,
	FUNC_DTR0,
	FUNC_DSR0,
	FUNC_DCD0,
	FUNC_CTS0,

	FUNC_CT32B0_CAP0,
	FUNC_CT32B0_MAT0,
	FUNC_CT32B0_CAP1,
	FUNC_CT32B0_MAT1,
	FUNC_CT32B0_CAP2,
	FUNC_CT32B0_MAT2,
	FUNC_CT32B0_CAP3,
	FUNC_CT32B0_MAT3,

	FUNC_CT32B1_CAP0,
	FUNC_CT32B1_MAT0,
	FUNC_CT32B1_CAP1,
	FUNC_CT32B1_MAT1,
	FUNC_CT32B1_CAP2,
	FUNC_CT32B1_MAT2,
	FUNC_CT32B1_CAP3,
	FUNC_CT32B1_MAT3,

	FUNC_CT16B0_CAP0,
	FUNC_CT16B0_MAT0,
	FUNC_CT16B0_CAP1,
	FUNC_CT16B0_MAT1,

	FUNC_CT16B1_CAP0,
	FUNC_CT16B1_MAT0,
	FUNC_CT16B1_CAP1,
	FUNC_CT16B1_MAT1,

	FUNC_RI0,
	FUNC_SCL,
	FUNC_SDA,

	FUNC_CLKOUT,

	FUNC_SCK,
	FUNC_SSEL,
	FUNC_MISO,
	FUNC_MOSI,

	FUNC_ADC,

	FUNC_SWCLK,
	FUNC_SWDIO,

	FUNC_RESET,

	FUNC_ROSC

} gpio_func_types;

////////////////////////////////////////////////////////////////
//
// Offsets within a pin's IOCONFIG register, used by the macros below
//
#define	PIN_FUNC		0
#define	PIN_MODE		4
#define	PIN_INV			6
#define	PIN_ADMODE		7
#define	PIN_DRV			9
#define	PIN_OD			10
#define	PIN_S_MODE		11
#define	PIN_CLK_DIV		13

////////////////////////////////////////////////////////////////
//
// Constants used when calling the pinMode() function
//
#define	PIN_MODE_NOPULLUP		(0 << PIN_MODE)
#define	PIN_MODE_PULLUP			(1 << PIN_MODE)

#define	PIN_INV_NO				(0 << PIN_INV)
#define	PIN_INV_YES				(1 << PIN_INV)

#define	PIN_ADMODE_ENABLED		(0 << PIN_ADMODE)
#define	PIN_ADMODE_DISABLED		(1 << PIN_ADMODE)

#define	PIN_DRIVE_LOW			(0 << PIN_DRV)
#define	PIN_DRIVE_low			(1 << PIN_DRV)	// TODO: check this

#define	PIN_OD_DISABLED			(0 << PIN_OD)
#define	PIN_OD_ENABLED			(1 << PIN_OD)

#define	PIN_S_MODE_BYPASS		(0 << PIN_S_MODE)
#define	PIN_S_MODE_1CLK			(1 << PIN_S_MODE)
#define	PIN_S_MODE_2CLK			(2 << PIN_S_MODE)
#define	PIN_S_MODE_3CLK			(3 << PIN_S_MODE)

#define	PIN_CLK_DIV0			(0 << PIN_CLK_DIV)
#define	PIN_CLK_DIV1			(1 << PIN_CLK_DIV)
#define	PIN_CLK_DIV2			(2 << PIN_CLK_DIV)
#define	PIN_CLK_DIV3			(3 << PIN_CLK_DIV)
#define	PIN_CLK_DIV4			(4 << PIN_CLK_DIV)
#define	PIN_CLK_DIV5			(5 << PIN_CLK_DIV)
#define	PIN_CLK_DIV6			(6 << PIN_CLK_DIV)

////////////////////////////////////////////////////////////////
//
// Values used when calling _pinSetGpioBit() or _pinClearGpioBit()
// Used to index into the gpio_regs array.
//
typedef enum  {
	GPIOREG_MASK,
	GPIOREG_PIN,
	GPIOREG_OUT,
	GPIOREG_SET,
	GPIOREG_CLR,
	GPIOREG_NOT,
	GPIOREG_DIR,
	GPIOREG_IS,
	GPIOREG_IBE,
	GPIOREG_IEV,
	GPIOREG_IE,
	GPIOREG_RIS,
	GPIOREG_MIS,
	GPIOREG_IC,
} gpio_regs_offsets;

////////////////////////////////////////////////////////////////
//
// Values used when setting the interrupt mode of a pin
//
enum {
	PININT_HIGH_LEVEL,
	PININT_LOW_LEVEL,
	PININT_RISING_EDGE,
	PININT_FALLING_EDGE,
	PININT_BOTH_EDGES
};

#if 0
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
#ifdef LPC1227_64PIN
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
#endif

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
#ifdef LPC1227_64PIN
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
#endif

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
#ifdef LPC1227_64PIN
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
#endif

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
#ifdef LPC1227_64PIN
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
#endif

#endif

#endif /* PIN_H_ */
