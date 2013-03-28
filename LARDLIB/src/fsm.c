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
