/////////////////////////////// events.c //////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

voidFuncPtr 	eventFunctions[EVENT_DUMMY] = {0};
static uint32	__n_systick_handlers	= 0;

const uint32 N_EVENTS = (sizeof(eventFunctions) / sizeof(eventFunctions[0]));

uint32 attachEventHandler (uint32 event, void (*userFunc)(void)) {

	if (event >= N_EVENTS) {
		SYS_ERROR(ERR_INV_INTERRUPT);
		return ERROR;
	}

	//ASSERT_RETERR (event >= N_EVENTS, ERR_INV_INTERRUPT);

	/////////////////////////////////////////////////////////////
	//
	// The systick handler is a special case in that we allow up
	// to four of them in any order. They are allocated on the
	// order of the attachEventHandler() calls with the default
	// handler being the first as it's allocated in init()
	//
	if (event == EVENT_SYS_TICK) {
		ASSERT_RETERR (__n_systick_handlers < N_SYSTICK_HANDLERS-1, ERR_TOO_MANY_SYSTICKS);
		event = __n_systick_handlers++;
	}

	ATOMIC(eventFunctions[event] = userFunc;)

	////////////////////////////////////////////////////////////
	// we return event in case a cod ethat sets a systick handler
	// wants to know which slot was allocated
	return event;
}

uint32 detachEventHandler(uint32 event) {

	if (event >= N_EVENTS) {
		SYS_ERROR(ERR_INV_INTERRUPT);
		return ERROR;
	}

	ATOMIC(eventFunctions[event] = NULL;)

	return NOERROR;

}

//////////////////////////////////////////////////////////////////////
//
// This is the default handler for the SysTick timer, it overrides the
// weak definition provided by the LPC startup code.
//
void SysTick_Handler () {

	msTicks++;

	//////////////////////////////////////////
	// Default handler in main.c
	if (eventFunctions[EVENT_SYS_TICK] != 0)
		(eventFunctions[EVENT_SYS_TICK]) ();

	//////////////////////////////////////////
	// Possible user-provided handlers
	if (eventFunctions[EVENT_SYS_TICK0] != 0)
		(eventFunctions[EVENT_SYS_TICK0]) ();

	if (eventFunctions[EVENT_SYS_TICK1] != 0)
		(eventFunctions[EVENT_SYS_TICK1]) ();

	if (eventFunctions[EVENT_SYS_TICK2] != 0)
		(eventFunctions[EVENT_SYS_TICK2]) ();
}


