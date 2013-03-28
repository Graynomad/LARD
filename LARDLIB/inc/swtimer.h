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
//////////////////////////////// swtimer.h ////////////////////////////
//
//
//
//
//

#ifndef SWTIMER_H_
#define SWTIMER_H_

#define N_SWTIMERS 32


typedef enum {
	SWTIMER_TYPE_MONOSTABLE,	// Timer is not reloaded on timeout
	SWTIMER_TYPE_ASTABLE		// Timer is reloaded on timeout
} swTimer_types;

typedef struct _swTimer swTimer;

struct _swTimer {

	uint8	object_id;

	uint32	type;
	uint32	count;
	uint32	reload_value;
	uint32	max_reload_value;	// Traps values that are too large
	uint32	enabled;
	void 	(*callback_func)(struct _swTimer *);

	uint8	not_object_id;

} ;

uint32 swTimerAttachCallback 	(swTimer * t, void (*callback_func)(swTimer *));
void   swTimerExecuteCallbacks 	(void);
uint32 swTimerScan 				(void);
swTimer * swTimerCreate 		(uint32 reload_val, uint32 max_reload_val, uint32 type, void (*callback_func)(swTimer *));
uint32 swTimerSetReLoadVal		(swTimer * t, uint32 reload_val);
uint32 swTimerReload 			(swTimer * t);
uint32 swTimerStart				(swTimer * t);
uint32 swTimerRestart 			(swTimer * t);
uint32 swTimerStop				(swTimer * t);

#endif /* SWTIMER_H_ */
