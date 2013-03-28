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
/////////////////////////////// adc.c //////////////////////////////////
//
//
// TODO: Currently the ADC is enabled by the call to adcInit() in main()
//		 This should be changed to only enable the ADC when a pin is
// 		 set to be an analog input.
//
// TODO: Modify to always use the interrupt-driven option.
//

#include "LARD.h"

volatile uint32_t ADCValue[ADC_NUM];
volatile uint32_t ADCIntDone = 0;

#if BURST_MODE
volatile uint32_t channel_flag; 
#endif


#if ADC_INTERRUPT_FLAG
/******************************************************************************
** Function name:		ADC_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_IRQHandler (void) 
{
  uint32_t regVal;
  
  regVal = LPC_ADC->STAT;		/* Read ADC will clear the interrupt */
  if ( regVal & 0x0000FF00 )	/* check OVERRUN error first */
  {
	regVal = (regVal & 0x0000FF00) >> 0x08;
	/* if overrun, just read ADDR to clear */
	/* regVal variable has been reused. */
	switch ( regVal )
	{
	  case 0x01:
		regVal = LPC_ADC->DR[0];
		break;
	  case 0x02:
		regVal = LPC_ADC->DR[1];
		break;
	  case 0x04:
		regVal = LPC_ADC->DR[2];
		break;
	  case 0x08:
		regVal = LPC_ADC->DR[3];
		break;
	  case 0x10:
		regVal = LPC_ADC->DR[4];
		break;
	  case 0x20:
		regVal = LPC_ADC->DR[5];
		break;
	  case 0x40:
		regVal = LPC_ADC->DR[6];
		break;
	  case 0x80:
		regVal = LPC_ADC->DR[7];
		break;
	  default:
		break;
	}
	LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	ADCIntDone = 1;
	return;	
  }
    
  if ( regVal & ADC_ADINT )
  {
	switch ( regVal & 0xFF )	/* check DONE bit */
	{
	  case 0x01:
		ADCValue[0] = ( LPC_ADC->DR[0] >> 6 ) & 0x3FF;
	    break;
	  case 0x02:
		ADCValue[1] = ( LPC_ADC->DR[1] >> 6 ) & 0x3FF;
	    break;
	  case 0x04:
		ADCValue[2] = ( LPC_ADC->DR[2] >> 6 ) & 0x3FF;
	    break;
	  case 0x08:
		ADCValue[3] = ( LPC_ADC->DR[3] >> 6 ) & 0x3FF;
	    break;
	  case 0x10:
		ADCValue[4] = ( LPC_ADC->DR[4] >> 6 ) & 0x3FF;
	    break;
	  case 0x20:
		ADCValue[5] = ( LPC_ADC->DR[5] >> 6 ) & 0x3FF;
	    break;
	  case 0x40:
		ADCValue[6] = ( LPC_ADC->DR[6] >> 6 ) & 0x3FF;
	    break;
	  case 0x80:
		ADCValue[7] = ( LPC_ADC->DR[7] >> 6 ) & 0x3FF;
	    break;		
	  default:
	    break;
	}
#if BURST_MODE
	channel_flag |= (regVal & 0xFF);
	if ( (channel_flag & 0xFF) == 0xFF )
	{
	  /* All the bits in have been set, it indicates all the ADC 
	  channels have been converted. */
	  LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	  ADCIntDone = 1;
	}
#else
	LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	ADCIntDone = 1;
#endif
  }
  return;
}
#endif


/////////////////////////////////////////////////////////////////////
//
// Function name:		adcInit
//
// Description:			Initialise the ADC
//
// Parameters:			uint32 clockRate, The ADC clock rate
//
// Returned value:		NOERROR
//
// Errors raised:		none
//
// Notes:				Does not set any pins to analog input mode, this must
//						be done externally with a call to pinSetAsAnalogInput()
//
// Example:
//
uint32 adcInit(uint32 clockRate) {
	uint32 i;

	///////////////////////////////////////////////////////////
	// Enable Power To ADC
	LPC_SYSCON->PDRUNCFG &= ~(1<<4);

	///////////////////////////////////////////////////////////
	// Enable AHB clock
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<14);

#if ADC_INTERRUPT_FLAG
	for ( i = 0; i < ADC_NUM; i++ ) {
		ADCValue[i] = 0x0;
	}
#endif

	i = ((SystemCoreClock/clockRate)-1)&0xFF;

	LPC_ADC->CR = ( 0x01 << 0 ) | 	// SEL=1,select channel 0~7 on ADC0
		( i << 8 )  |          		// CLKDIV = Fpclk / 1000000 - 1
		( 0 << 16 ) | 				// BURST = 0, no BURST, software controlled
		( 0 << 24 ) |  				// START = 0 A/D conversion stops
		( 0 << 27 );				// EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion)

	return NOERROR;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t adcRead( uint8_t channel )
{
#if !ADC_INTERRUPT_FLAG
  uint32_t regVal, ADC_Data;
#endif

	if ((channel > 7) && (channel < A0) )  {
		SYS_ERROR (ERR_BAD_ADC_CHANNEL | channel);
		return ERROR;
	}

////////////////////////////////////////////////////////////////
//
// Channel numbers are 0 through 7 but caller may have used the
// A0-A7 #defines so change to channel number
//
	if (channel >= A0)  {
		channel -= A0;
	}

	LPC_ADC->CR &= 0xFFFFFF00;
	LPC_ADC->CR |= (1 << 24) | (1 << channel);
						/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
	while ( 1 ) {			// wait until end of A/D convert
		// read result of A/D conversion
		regVal = *(volatile unsigned long *)(LPC_ADC_BASE + ADC_OFFSET + ADC_INDEX * channel);
		if ( regVal & ADC_DONE ) {
		  break;
		}
	}

	LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */
	if ( regVal & ADC_OVERRUN )	{ // save data when it's not overrun, otherwise, return zero
		return ( 0 );
	}

	ADC_Data = ( regVal >> 6 ) & 0x3FF;
	return ( ADC_Data );		/* return A/D conversion value */

#else
  return ( channelNum );	/* if it's interrupt driven, the ADC reading is 
							done inside the handler. so, return channel number */
#endif
}

/*****************************************************************************
** Function name:		ADC0BurstRead
**
** Descriptions:		Use burst mode to convert multiple channels once.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void adcBurstRead( void )
{
  /* START bits must be 000 when BURST = 1 or conversions will not start. */
  LPC_ADC->CR &= ~(0x7<<24);
  
  LPC_ADC->CR &= ~0xFF;
#ifdef __JTAG_DISABLED
  /* Read all channels, 0 through 7. */
  LPC_ADC->CR |= 0xFF;
#else
  /* Test channel 5,6,7 using burst mode because they are not shared
  with the JTAG pins. */
  LPC_ADC->CR |= 0xFF;
#endif
  LPC_ADC->CR |= (0x1<<16);		/* Set burst mode and start A/D convert */
  return;						/* the ADC reading is done inside the 
								handler, return 0. */
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
