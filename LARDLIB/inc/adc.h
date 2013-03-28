/////////////////////////////////// adc.h /////////////////////////////////
//
//
//
//
//

#ifndef __ADC_H 
#define __ADC_H

#define ADC_INTERRUPT_FLAG	0   // 1 is interrupt driven, 0 is polling
#define BURST_MODE		0   	// Burst mode works in interrupt driven mode only.
#define ADC_DEBUG		0

#define ADC_OFFSET		0x10	// index to starting addr of DR0~DR7 register in ADC
#define ADC_INDEX		4

#define UART_NUM		0

#define ADC_DONE		0x80000000
#define ADC_OVERRUN		0x40000000
#define ADC_ADINT		0x00010000

//////////////////////////////////////////////////////////////////////
//
// Analog channel to pin mapping
//
#define	A0				30
#define	A1				31
#define	A2				32
#define	A3				33
#define	A4				34
#define	A5				35
#define	A6				36
#define	A7				37

#define ADC_NUM			8
#define ADC_CLK			1000000

extern void 	ADC_IRQHandler	( void );
extern uint32 	adcInit			( uint32 clockRate);
extern uint32_t adcRead			( uint8_t channelNum );
extern void 	adcBurstRead	( void );

#endif /* __ADC_H */
