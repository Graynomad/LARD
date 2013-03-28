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
///////////////////////////// hwtimer.c //////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

///////////////////////////////////////////////////////////////////////
//
// Because hwTimers are dynamically allocated there is no static way to
// reference them and the ISR needs pointers to pass a timer reference  to
// the user's event handler.
//
// This array holds pointers to any hwTimers that have been created.
//
hwTimer * hwTimers[N_HWTIMERS] = {0};
//hwTimer * uartConnections [N_HWTIMERS];

///////////////////////////////////////////////////////////////////////
//
// Some default values to put into a hwTimer structure,
// used by hwTimerCreate()
//
hw_timer_parms timer_parms[N_HWTIMERS] = {
		{LPC_CT16B0, TIMER_16_0_IRQn, 0xFFFF},		// HWTIMER_0
		{LPC_CT16B1, TIMER_16_1_IRQn, 0xFFFF},		// HWTIMER_1
		{LPC_CT32B0, TIMER_32_0_IRQn, 0xFFFFFFFF},	// HWTIMER_2
		{LPC_CT32B1, TIMER_32_1_IRQn, 0xFFFFFFFF}	// HWTIMER_3
};


void TIMER16_0_IRQHandler() {      // 16-bit Timer0

	LPC_CT16B0->IR = 1;
	hwTimer * t = hwTimers[HWTIMER_0];

	if (t->mode == HWTIMER_MODE_MONOSTABLE) t->state = HWTIMER_STATE_OFF;

	hwTimerFuncPtr f = (void*)t->callback_func;
	if (f != NULL) t->callback_func(t);
}

void TIMER16_1_IRQHandler() {      // 16-bit Timer1

	LPC_CT16B1->IR = 1;
	hwTimer * t = hwTimers[HWTIMER_1];

	if (t->mode == HWTIMER_MODE_MONOSTABLE) t->state = HWTIMER_STATE_OFF;

	hwTimerFuncPtr f = (void*)t->callback_func;
	if (f != NULL) t->callback_func(t);
}

void TIMER32_0_IRQHandler (void) {	// 32-bit Timer0

	LPC_CT32B0->IR = 1;
	hwTimer * t = hwTimers[HWTIMER_2];

	if (t->mode == HWTIMER_MODE_MONOSTABLE) t->state = HWTIMER_STATE_OFF;

	hwTimerFuncPtr f = (void*)t->callback_func;
	if (f != NULL) t->callback_func(t);
}

void TIMER32_1_IRQHandler(void) {		 // 32-bit Timer1

	LPC_CT32B1->IR = 1;
	hwTimer * t = hwTimers[HWTIMER_3];

	if (t->mode == HWTIMER_MODE_MONOSTABLE) t->state = HWTIMER_STATE_OFF;

	hwTimerFuncPtr f = (void*)t->callback_func;
	if (f != NULL) t->callback_func(t);
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerCreate
//
// Description:			Create and populate a new hwTimer object.
//
// Parameters:			uint8 tmr, ordinal number of the timer, 0-N_HWTIMERS
//						uint32 reload_val, value placed in the match register, can
//							if up to 0xFFFF for timers 0 ans 1 and 0xFFFFFFFF for
//							timers 2 and 3.
//						uint8 mode, HWTIMER_MODE_ASTABLE (continue counting) or
//									HWTIMER_MODE_MONOSTABLE (only count once).
//						void (*callback_func)(struct _hwTimer *), a pointer to a
//							user function that will be called by the default timer
//							ISR. This function will receive a pointer to the timer.
//						uint32 prescale_val, a value loaded into the PR register.
//							This value divided the system clock and can be any number
//							up to max_val. Two pre-defined values are provided,
//							HWTIMER_COUNT_RATE_US and HWTIMER_COUNT_RATE_MS that
//							cause the timer to count uS and mS.
//
// Returned value:		ERROR if a problem was encountered
//						Pointer to the new hwTimer object if not
//
// Errors raised:		ERR_MALLOC_FAILED if memory allocation failed.
//						ERR_HWTIMER_BAD_RELOAD_VAL if reload_val was invalid, ei
//							larger than the default max_val
//						ERR_HWTIMER_BAD_PRESCALE_VAL if prescale_val was invalid, ei
//							larger than the default max_val
//						ERR_HWTIMER_BAD_MODE if the mode parm was not one of the
//							two valid balues
//
// Example:				hwTimer * hwt;
//						hwt = hwTimerCreate (HWTIMER_0, 1000, HWTIMER_MODE_ASTABLE,
//												hwTimerFunc, HWTIMER_COUNT_RATE_MS);
//
//						// The function hwTimerFunc will be called every 1000mS.
// Notes:
//
hwTimer * hwTimerCreate (uint8 tmr, uint32 reload_val, uint8 mode,
		void (*callback_func)(struct _hwTimer *), uint32 prescale_val) {

	hwTimer * t;

	t = (void*)safeMalloc(sizeof (hwTimer));

	if (t == NULL) {
		SYS_ERROR (ERR_MALLOC_FAILED);
		return (hwTimer *)ERROR;
	}

	t->object_id		= OBJID_HWTIMER;
	t->not_object_id	= ~OBJID_HWTIMER;

	t->timer 			= timer_parms[tmr].timer;
	t->mode 			= mode;
	t->callback_func	= callback_func;
	t->int_number		= timer_parms[tmr].interrupt_number;
	t->tmr_number		= tmr;
	t->state			= HWTIMER_STATE_OFF;

	hwTimers[tmr] = t;
	//attachEventHandler (EVENT_HWTIMER0_MATCH + tmr, (void*)callback_func);

	/////////////////////////////////////////////////////////////////
	//
	// Enable the appropriate AHB clock, the bits in SYSAHBCLKCTRL
	// are 7, 8, 9 and 10 for the four timers, so we use the tmr var
	// added to the first bit position.
	//
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << (7 + tmr));

	/////////////////////////////////////////////////////////////////
	//
	// Disable and reset the timer.
	//
	t->timer->TCR = 2;

	/////////////////////////////////////////////////////////////////
	//
	// Set default max values the timer or prescaler can have
	//
	// Note that sw_max_val can be overridden later with the
	// hwTimerSetMaxVal() function.
	//
	t->hw_max_val = timer_parms[tmr].max_val;
	t->sw_max_val = t->hw_max_val;

	/////////////////////////////////////////////////////////////////
	//
	// Setup reload value
	//
	if (reload_val > t->sw_max_val) {
		SYS_ERROR (ERR_HWTIMER_BAD_RELOAD_VAL);
		reload_val &= timer_parms[tmr].max_val;
	}
	t->reload_val 	= reload_val;	// save in structure
	t->timer->MR0	= reload_val;	// save in timer's match register

	/////////////////////////////////////////////////////////////////
	//
	// Setup the prescaler value
	//
	if (prescale_val > t->hw_max_val) {
		SYS_ERROR (ERR_HWTIMER_BAD_PRESCALE_VAL);
		prescale_val &= timer_parms[tmr].max_val;
	}
	t->timer->PR = prescale_val;	// save in prescaler register

	/////////////////////////////////////////////////////////////////
	//
	// Reset the any possible existing interrupt for MRxINT.
	//
	t->timer->IR = 1;

	/////////////////////////////////////////////////////////////////
	//
	// Reset the counter and set the match register action to
	// generate an interrupt and restart or stop according to mode.
	//
	if ((mode != HWTIMER_MODE_ASTABLE) && (mode != HWTIMER_MODE_MONOSTABLE)) {
		SYS_ERROR (ERR_HWTIMER_BAD_MODE);
		mode &= HWTIMER_MODE_ASTABLE;
	}
	t->timer->MCR =	(1 << 0) | 	// generate interrupt on MCR0 match
					(1 << 1) |	// reset counter
					mode;		// stop if 4, restart if 0

	/////////////////////////////////////////////////////////////////
	//
	// Ensure timer mode
	//
	t->timer->CTCR 	= 0;	// timer
	t->timer->PWMC	= 0;	// no PWM ???????
	t->timer->TCR	= 2;	// reset timer

	return t;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerAttachCallback
//
// Description:			Provide a callback function for the timer's match
//						interrupt.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//						void (*callback_func)(struct _hwTimer *) pointer to
//							the callback function.
//
// Returned value:		Hardcoded to NOERROR.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//
// Example:
//
// Notes:				The low-level ISR for timer matches are called
//						TIMERxx_y_IRQHandler() where xx is 16 or 32 and y is
//						0 or 1. These function can be found in interrupts.c
//
//						These low-level ISRs clear the interrupt then check for
//						a user-supplied event handler function in the eventFunctions[]
//						array. If one is found it is called from the ISR.
//
uint32 hwTimerAttachCallback (hwTimer * t, void (*callback_func)(struct _hwTimer *)) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	ATOMIC (t->callback_func = callback_func;)

	return NOERROR;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerSetReLoadVal
//
// Description:			Loads the timers counter value.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//						uint32 new_reload_val, new timer value
//
// Returned value:		Hardcoded to NOERROR.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//						ERR_HWTIMER_BAD_RELOAD_VAL if new_reload_val is
//							larger than hw_max_val.
//
// Example:
//
// Notes:				Loads new_reload_val into the timer structure and
//						also the match 0 register. Also sets the timer's
//						TC (timer/counter) register to 0. if the timer was
//						running at the time of the call it will continue
//						to run starting back at 0. if the timer was not
//						there is no immediate effect.
//
uint32	hwTimerSetReLoadVal (hwTimer * t, uint32 new_reload_val) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	if (new_reload_val > t->hw_max_val) {
		SYS_ERROR (ERR_HWTIMER_BAD_RELOAD_VAL);
		new_reload_val &= t->hw_max_val;
	}

	ATOMIC (
		t->reload_val	= new_reload_val;
		t->timer->MR0	= new_reload_val;
		t->timer->TC	= 0;
	)

	return NOERROR;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerSetMaxVal
//
// Description:			Set a new software-restricted max timer value.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//						uint32 new_max_val, the new software-restricted
//							max timer value
//
// Returned value:		Hardcoded to NOERROR.
//						ERR_HWTIMER_BAD_MAX_VAL if new_max_val is
//							larger than hw_max_val.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//
// Example:
//
// Notes:				The value set by this function is used to restrict
//						any future writes to either the prescaler or the timer
//						registers.
//
uint32	hwTimerSetMaxVal (hwTimer * t, uint32 new_max_val) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	if (new_max_val > t->hw_max_val) {
		SYS_ERROR (ERR_HWTIMER_BAD_MAX_VAL);
		new_max_val &= t->hw_max_val;
	}

	ATOMIC (t->sw_max_val	= new_max_val;)

	return NOERROR;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerStart
//
// Description:			Start a timer.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//
// Returned value:		Hardcoded to NOERROR.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//
// Example:
//
// Notes:				The timer's match register is loaded with reload_val,
//						the timer/counter register is reset to 0, the timer's
//						hardware is enabled and any pending interrupts cleared.
//						The NVIC is also updated to enable the timer's interrupt.
//
uint32	hwTimerStart (hwTimer * t) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	t->timer->MR0	= t->reload_val;// match reg
	t->timer->TC	= 0;			// timer/counter reg
	t->timer->TCR 	= 1;			// enable hardware
	t->timer->IR	= 1;			// clear any pending interrupt
	t->state		= HWTIMER_STATE_TIMING;

	NVIC_EnableIRQ (t->int_number);	// enable interrupt in the NVIC

	return NOERROR;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerRestart
//
// Description:			Restart the timer hardware.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//
// Returned value:		Hardcoded to NOERROR.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//
// Example:
//
// Notes:				This function just enables the timer's counter.
//						If all else is enabled it will start counting
//						from the current value in the timer/counter
//						register.
//
uint32	hwTimerRestart (hwTimer * t) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	t->timer->TCR 	= 1;

	return NOERROR;

}

////////////////////////////////////////////////////////////////////
//
// Function name:		hwTimerStop
//
// Description:			Stop the timer hardware.
//
// Parameters:			hwTimer * t, pointer to a hwTimer object.
//
// Returned value:		Hardcoded to NOERROR.
//
// Errors raised:		Fatal error if the VERIFY_OBJECT macro fails.
//
// Example:
//
// Notes:				The timer's hardware is disabled but no values
//						are changed. A subsequent call to hwTimerRestart()
//						will allow the timer to resume from where it
//						left off.
//
uint32	hwTimerStop (hwTimer * t) {

	VERIFY_OBJECT(t, OBJID_HWTIMER)

	t->timer->TCR 	= 0;
	t->state		= HWTIMER_STATE_OFF;

	return NOERROR;

}

uint32	hwTimerIsTiming	(hwTimer * t) {

	VERIFY_OBJECT(t, OBJID_HWTIMER);

	return t->state == HWTIMER_STATE_TIMING;

}
