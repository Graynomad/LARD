///////////////////////////////// fsm.c /////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

Fsm * fsmCreate (uint32 states, uint32 events, uint32 q_size,
						uint32 state, fsmFuncPtr * action_table) {

	Fsm * fsm;

	/////////////////////////////////////////////////////////////////
	// Create the fsm structure
	fsm = (void*)safeMalloc(sizeof (Fsm));

	if (fsm == NULL) {
		SYS_ERROR (ERR_SERIAL_INIT_FAILED | 2);
		return (Fsm *)ERROR;
	}

	fsm->q_events = fifo32Create(q_size);
	if (fsm->q_events == NULL) {
		safeFree (fsm);
		SYS_ERROR (ERR_SERIAL_INIT_FAILED | 4);
		return (Fsm *)ERROR;
	}

	fsm->state 		= state;
	fsm->actions	= action_table;
	fsm->n_states	= states;
	fsm->n_events	= events;

	fsm->object_id		= OBJID_FSM;
	fsm->not_object_id	= ~OBJID_FSM;

	return fsm;

}

uint32 fsmRaiseEvent (Fsm * fsm, uint32 event, uint32 val) {

	VERIFY_OBJECT(fsm, OBJID_FSM);

	if (event >= fsm->n_events) {
		SYS_ERROR (ERR_BAD_FSM_EVENT | event);
		return ERROR;
	}

	fifo32Write (fsm->q_events, (val << 8) | event);

	return NOERROR;

}

void fsmHandleEvents (Fsm * fsm) {
	uint32 		event;
	uint32		offset;
	fsmEvent 	e;
	uint32		returned_state;

	VERIFY_OBJECT(fsm, OBJID_FSM);

	while (fsm->q_events->nItems) {
		event = fifo32Read (fsm->q_events, NULL);

		e.val = event >> 8;
		e.event = event &= 0xFF;

		offset = (fsm->state * fsm->n_events) + e.event;

		fsmFuncPtr f = *(fsm->actions+offset);
		if (f != NULL) returned_state = f(e);
//		returned_state = (*(fsm->actions+offset))(e);

		if (returned_state >= fsm->n_states) {
			SYS_ERROR (ERR_BAD_FSM_STATE | returned_state);
		} else {
			fsm->state = returned_state;
		}
	}
}
