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
//////////////////////////////// fsm.h ////////////////////////////
//
//
//
//
//

#ifndef FSM_H_
#define FSM_H_



typedef struct _fsmEvent fsmEvent;

struct _fsmEvent {

	uint8 	event 	: 8;
	uint32	val 	: 24;

};

/////////////////////////////////////
// This typedef is duplicated in lard.h.
// Duplication required to fix circular forward references
typedef uint32 	(*fsmFuncPtr)	(fsmEvent e);

typedef struct _fsm Fsm;

struct _fsm {

	uint8			object_id;

	uint8			state;
	uint8			n_events;
	uint8			n_states;

	fifo32 * 		q_events;
	fsmFuncPtr *	actions;

	uint8			not_object_id;

};

void 	fsmHandleEvents 	(Fsm * fsm);
Fsm *	fsmCreate 			(uint32 states, uint32 events, uint32 q_size,
									uint32 state, fsmFuncPtr * actions);
uint32 	fsmRaiseEvent 		(Fsm * fsm, uint32 event, uint32 val);

#define 	FSM_TABLE_BEGIN(n, s, e) 	 fsmFuncPtr n[s][e] = {
#define		FSM_STATE_BEGIN					{
#define		FSM_ACTION(a)					a,
#define		FSM_STATE_END					},
#define 	FSM_TABLE_END					};

#endif /* FSM_H_ */
