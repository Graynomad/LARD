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
////////////////////////////// swtimer.c ////////////////////////////
//
// A collection of functions that implement software timers with a 1mS
// tick resolution.
//


#include "LARD.h"

static uint8 	__n_swTimers = 0;

static uint32 	__timer_flags = 0;
static swTimer * swTimers [N_SWTIMERS];

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerScan
//
// Description:			Scan all timers decrementing their counter,
//						If the counter reaches 0 reload the counter if
//						the timer is an SWTIMER_TYPE_ASTABLE type.
//
// Parameters:			none
//
// Returned value:		> 0 if any timers timed out, 0 if not
//
// Errors raised:		none
//
// Example:
//
// Notes:				Sets a bit in __timer_flags for each timer
//						that times out.
//
//						This function is called automatically by sysTickHandler()
//						and should not normally be used elsewhere.
//
uint32 swTimerScan (void) {

	uint32	flag_mask = 1;
	swTimer	* t;
	boolean	flag = FALSE;

	if (__n_swTimers > N_SWTIMERS) {
		FATAL (ERR_CORRUPT_VARIABLE | VAR_N_SWTIMERS)
	}

	if (__n_swTimers) {
		__timer_flags = 0;

		for (int i = 0; i < __n_swTimers; i++) {
			t = swTimers[i];
			VERIFY_OBJECT(t, OBJID_SWTIMER)		// TODO: this test does not work
			if (t->enabled) {
				t->count--;
				if (t->count == 0) {
					if (t->type == SWTIMER_TYPE_ASTABLE)
						t->count = t->reload_value;
					else
						t->enabled = FALSE;
					flag = TRUE;
					__timer_flags |= flag_mask;  // TODO: add check for timer overrun
				}
			}
			flag_mask <<= 1;
		}
	}
	return (flag);
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerExecuteCallbacks
//
// Description:			Scan the __timer_flags variable looking for
//						set bits. When found call the appropriate
//						timer's callback function.
//
// Parameters:			none
//
// Returned value:		none
//
// Errors raised:		none
//
// Example:				swTimerExecuteCallbacks();
//
// Notes:				__timer_flags cleared by this function.
//
//						swTimerExecuteCallbacks() called automatically every
//						by main() in the while loop that also calls loop().
//						As such any excessive delays in loop() will impact
//						the swTimers.
//
void swTimerExecuteCallbacks (void) {

	uint32 flags;
	swTimer	* t;

	ATOMIC_START
		flags = __timer_flags;
		__timer_flags = 0;
	ATOMIC_END

	if (flags) {
		for (int i = 0; swTimers[i] != NULL && i < N_SWTIMERS; i++) {
			t = swTimers[i];
			if ((flags & 1) && ((t->callback_func) != NULL) ) {
				(t->callback_func)(t);
			}
			flags >>= 1;
		}
	}
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerAttachCallback
//
// Description:			Set the callback function for a timer.
//
// Parameters:			swTimer * t, pointer to the timer
//						void (*callback_func)(swTimer *), pointer to
// 							the timer's	callback function
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
// Example:				myFunc () {
// 							// do this when timer 1 times out
//						}
//						...
//						swTimerAttachCallback (1, myFunc);
//
// Notes:				Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
uint32 swTimerAttachCallback (swTimer * t, void (*callback_func)(swTimer *)) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	ATOMIC (t->callback_func = callback_func;)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerSetReLoadVal
//
// Description:			Load a timer's reload value.
//
// Parameters:			swTimer * t, pointer to the timer
//						uint32 reload_val, the timer's reload_value in mS
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
//						ERR_SWTIMER_BAD_RELOAD_VAL if the new reload
//						value is 0 or too large ie > the max_reload_val
//						set	when the timer was created.
//
// Example:
//
// Notes:				This will have no effect on the timer's
//						current count. The loaded value will only
//						come into play when the timer next times out.
//
//						If this reload_val is > the max_reload_val set
//						when the timer was created no timer values are modified.
//
//						Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
uint32 swTimerSetReLoadVal (swTimer * t, uint32 reload_val) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	if ((reload_val == 0) || (reload_val > t->max_reload_value)) {
		SYS_ERROR (ERR_SWTIMER_BAD_RELOAD_VAL);
		return ERROR;
	}

	ATOMIC (t->reload_value = reload_val;)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerReload
//
// Description:			Forces the timer's counter to the reload value.
//
// Parameters:			swTimer * t, pointer to the timer
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
// Example:
//
// Notes:				If the timer is enabled the next scan will
//						count from the reload value thus effectively
//						making the timer a retriggerable oneshot.
//
//						The timer's	enabled status is not modified.
//
//						Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
uint32 swTimerReload (swTimer * t) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	ATOMIC (t->count = t->reload_value)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerStart
//
// Description:			Enables a timer and reloads the reload value,
//						counting will resume from the reload value at
//						the next scan.
//
// Parameters:			swTimer * t, pointer to the timer
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
// Example:
//
// Notes:				Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
// TODO:				Add a delay value to allow timers to be staggered
//
uint32 swTimerStart (swTimer * t) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	ATOMIC (
		t->count = t->reload_value;
		t->enabled = true;
	)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerRestart
//
// Description:			Enables a timer, at the next scan counting will
//						resume from the current count value.
//
// Parameters:			swTimer * t, pointer to the timer
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
// Example:
//
// Notes:				Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
//						No timer values are modified.
//
uint32 swTimerRestart (swTimer * t) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	ATOMIC (t->enabled = true)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerStop
//
// Description:			Disables a timer so it's counter will not
//						be decremented in the next scan.
//
// Parameters:			swTimer * t, pointer to the timer
//
// Returned value:		Hardcoded to NOERROR at present
//
// Errors raised:		ERR_BAD_OBJECT if the t parameter pointed
//						to a corrupt structure.
//
// Example:
//
// Notes:				No timer values are modified, the timer
//						can be restarted and continue from where it was
//						stopped.
//
//						Executes the FATAL macro if the t parameter
//						pointed to a corrupt structure.
//
uint32 swTimerStop (swTimer * t) {

	VERIFY_OBJECT(t, OBJID_SWTIMER)

	ATOMIC (t->enabled = false)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		swTimerCreate
//
// Description:			Create and populate a new swTimer.
//
// Parameters:			uint32 reload_val, the timers count value in mS
//						uint32 max_reload_val, the maximum number the reload
//							value can be set to in future.
//						uint32 type,
//							SWTIMER_TYPE_MONOSTABLE if the timer should stop
//								on timeout
//							SWTIMER_TYPE_ASTABLE if the timer should be
//								reloaded and continue counting on timeout
//						void (*callback_func)(void), pointer to the callback function
//
// Returned value:		ERROR if an error was encountered,
//						Pointer to the new swTimer if not
//
// Errors raised:		ERR_MALLOC_FAILED if the safeMalloc failed.
//
// Example:				void myTimerFunc (swTimer * t) {
//							// do something when timer expires
//						}
//
//						swTimer * myTimer;
//						myTimer = swTimerCreate (1000, 0, SWTIMER_TYPE_ASTABLE, myTimerFunc);
//
//						myTimer will run every second with no further intervention
//
// Notes:				Can only be called from within the setup() function.
//						At any other time safeMalloc will fail and raise an error.
//
//						max_reload_val can be larger or smaller than reload_val, it
//						is used to trap possible setting of reload values that are too
//						large in future calls to swTimerSetReLoadVal(). Set this to 0
//						to trap all calls to swTimerSetReLoadVal() if they are not
//						allowed in your application.
//
//
swTimer * swTimerCreate (uint32 reload_val, uint32 max_reload_val, uint32 type,
		void (*callback_func)(swTimer *)) {

	swTimer * t;

	if (__n_swTimers >= N_SWTIMERS) {
		SYS_ERROR (ERR_TOO_MANY_SWTIMERS);
		return (swTimer *)ERROR;
	}

	t = (void*)safeMalloc(sizeof (swTimer));

	if (t == NULL) {
		SYS_ERROR (ERR_MALLOC_FAILED);
		return (swTimer *)ERROR;
	}

	t->object_id 		= OBJID_SWTIMER;
	t->not_object_id 	= ~OBJID_SWTIMER;

	t->reload_value  	= reload_val;
	t->max_reload_value = max_reload_val;
	t->count 		 	= reload_val;
	t->type 		 	= type;
	t->callback_func	= callback_func;
	t->enabled 		 	= FALSE;

	swTimers[__n_swTimers++] = t;

	return t;
}


