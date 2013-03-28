//////////////////////////// interrupts.c ////////////////////////////
//
//  
//
//
//

#include "LARD.h"

uint8	__interruptLevel = 0;

boolean	__force_interrupt_scan = 1; 	// 0 or 1, this value added to the pins counter
										// if 1 then the counter is incremented after
										// every interrupt thus causing the next scan to
										// start from the next pin. If 0 the next scan
										// starts from the same pin

void forceFullInterruptScan (boolean x) {

	__force_interrupt_scan = (x) ? 1 : 0;

}


#if 0
voidFuncPtr intFunctions[EVENT_DUMMY] = {0};

const uint32 N_INTERRUPTS = (sizeof(intFunctions) / sizeof(intFunctions[0]));


void attachInterrupt (uint32 interrupt, void (*userFunc)(void)) {

	if (interrupt >= N_INTERRUPTS) {
		SYS_ERROR(ERR_INV_INTERRUPT);
		return;
	}

	intFunctions[interrupt] = userFunc;
}

void detachInterrupt(uint32 interrupt) {

	if (interrupt >= N_INTERRUPTS) {
		SYS_ERROR(ERR_INV_INTERRUPT);
		return;
	}

	intFunctions[interrupt] = 0;

}
#endif


///////////////////////////////////////////////////////////////////
//
// These IRQHandlers are mostly dummy functions that override the
// default weak declarations and do nothing.
//
// As and when real handlers are written they will appear in an
// appropriate file and when this happens the declaration here
// is commented out and the file name added.

//void UART0_IRQHandler() {}    	// in serial.c
//void UART1_IRQHandler() {}

void WAKEUP_IRQHandler() {}			// 12 wakeup sources
void I2C_IRQHandler() {}            // I2C
void SSP_IRQHandler() {}            // SSP
void COMP_IRQHandler() {}           // Comparators 0() {void}1
void ADC_IRQHandler() {}            // A/D Converter
void WDT_IRQHandler() {}            // Watchdog timer
void BOD_IRQHandler() {}            // Brown Out Detect
void PIOINT2_IRQHandler() {}        // PIO INT2		Not on 48-pin chip
void DMA_IRQHandler() {}            // DMA
void RTC_IRQHandler() {}            // RTC

/////////////////////////////////////////////////////////////////////
//
// Function name:		PIOINTx_IRQHandler
//
// Description:			Replacement for the default PORT0 interrupt handler.
//
// Parameters:			none
//
// Returned value:		none
//
// Errors raised:		none
//
// Notes:				The code has to determine the pin that caused the interrupt.
//						This is done by scanning the value in the MIS register
//						looking for a 1 bit. When found the corresponding user handler
//						is called if it exists.
//
//						Whether or not the user handler exists the interrupt is cleared.
//
//						Only a single interrupt at a time is called so an active pin
//						cannot hog the system. This is implemented by maintaining a
//						counter across invocations so for example if pin 4 is handled
//						this time the scan starts from pin 5 next time.
//
//						This method however has a side effect of increasing the latency
//						considerably in systems that only have 1 or 2 external interrupts,
//						for example after handling a pin 4 interrupt the next invocation
//						has to scan 31 bits before arriving back at pin 4.
//
//						This is largely reduced by writing 0 to the __force_interrupt_scan
//						variable. In this case the next scan starts from the pin the
//						last scan ended on.
//
// TODO:				Implement a scan range to further reduce latency.
//
// Example:
//
void PIOINT0_IRQHandler() {

	static uint8 pin = 0;

	ATOMIC_START;

		uint8 x = PINS_ON_PORT0;
		uint32 misVal = LPC_GPIO0->MIS;	// get the masked interrupt flags for this port

		do {
			if (misVal & 1) {
				////////////////////////////////////////////////
				// Check for a user-defined handler, if one exists
				// then call it, clear the interrupt, increment the
				// pin counter and exit
				if (eventFunctions[EVENT_PININT_0 + pin] != NULL) {
					(eventFunctions[EVENT_PININT_0 + pin]) ();
					pinClearInterrupt(pin);
					pin = (pin > PINS_ON_PORT0 ? 0 : pin + __force_interrupt_scan);
					ATOMIC_END;
					return;
				}
				/////////////////////////////////////////////////
				// Clear the interrupt, if no user handler was called
				// the interrupt is lost.
				pinClearInterrupt(pin);
			}
			pin = (pin > PINS_ON_PORT0 ? 0 : pin +1);
			misVal >>= 1;
		} while (--x);

	ATOMIC_END;
}

void PIOINT1_IRQHandler() {

	static uint8 pin = 0;

	ATOMIC_START;

		uint8 x = PINS_ON_PORT1;
		uint32 misVal = LPC_GPIO1->MIS;	// get the masked interrupt flags for this port

		do {
			if (misVal & word_bits[pin]) {
				////////////////////////////////////////////////
				// Check for a user-defined handler, if one exists
				// then call it, clear the interrupt, increment the
				// pin counter and exit
				if (eventFunctions[EVENT_PININT_0 + 32 + pin] != NULL) {
					(eventFunctions[EVENT_PININT_0 + 32 + pin]) ();
					pinClearInterrupt(32 + pin);
					pin = (pin > PINS_ON_PORT1 ? 0 : pin + __force_interrupt_scan);
					ATOMIC_END;
					return;
				}
				/////////////////////////////////////////////////
				// Clear the interrupt, if no user handler was called
				// the interrupt is lost.
				pinClearInterrupt(pin);
			}
			pin = (pin > PINS_ON_PORT1 ? 0 : pin + 1);
		} while (--x);

	ATOMIC_END;
}

