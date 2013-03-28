//////////////////////////////// hwtimer.h ////////////////////////////
//
//
//
//
//
#ifndef TIMER_H_
#define TIMER_H_

#define N_HWTIMERS	4

///////////////////////////////////////////////////////////////////////
//
// Structure to hold some default values to put into a hwTimer structure,
// used by hwTimerCreate()
//
typedef struct _hw_timer_parms hw_timer_parms;

struct _hw_timer_parms {
	LPC_CTxxBx_Type * 	timer;
	uint8				interrupt_number;
	uint32				max_val;
};

///////////////////////////////////////////////////////////////////////
//
// NOTE: The value 23 causes the counter to count at 1.004MHz
//		 The value 24 causes the counter to count at 0.964MHz
//
//		 So best case we have .04% off the ideal frequency
//		 with an ambient temperature of 36C
//
#define	BASE_PRESCALER_VALUE	23

///////////////////////////////////////////////////////////////////////
//
// Some preset prescaler values used to have the timer coun in uS or mS
//
typedef enum {

	HWTIMER_COUNT_RATE_US = BASE_PRESCALER_VALUE,
	HWTIMER_COUNT_RATE_MS = BASE_PRESCALER_VALUE * 1000

} hwTimerPrescaleVals;

///////////////////////////////////////////////////////////////////////
//
// Enums for the four timers, used to index into the hwTimers[] and
// timer_parms[] arrays and probably other places
//
typedef enum {
	HWTIMER_0,
	HWTIMER_1,
	HWTIMER_2,
	HWTIMER_3
} hwTimerTypes;

///////////////////////////////////////////////////////////////////////
//
// The main hwTimer structure
//
typedef struct _hwTimer hwTimer;

struct _hwTimer {

	uint8			object_id;

	LPC_CTxxBx_Type * timer;			// address of the actual timer hardware
	uint8			int_number;			// value passed to NVIC_EnableIRQ()
	uint8			tmr_number;			// the logical timer number
	uint32			reload_val;			// value to load into the match register
	uint32			sw_max_val;			// software restricted max value that can be loaded
										// into the match register
	uint32			hw_max_val;			// hardware-limited max value that can be loaded
										// into the match register
	uint8			mode;				// mono- or astable
	void 			(*callback_func)(hwTimer * t); 	// pointer to the function to call from
													// the match ISR
	uint8			state;

	uint8			not_object_id;

};



///////////////////////////////////////////////////////////////////////
//
// Valid values for the "state" element
//
typedef enum {
	HWTIMER_STATE_OFF,
	HWTIMER_STATE_TIMING
} hwTimer_states;

///////////////////////////////////////////////////////////////////////
//
// Valid values for the "mode" element
//
typedef enum {
	HWTIMER_MODE_ASTABLE,		// Timer is reloaded on timeout
	HWTIMER_MODE_MONOSTABLE = 4	// Timer is not reloaded on timeout
} hwTimer_types;

///////////////////////////////////////////////////////////////////////
//
// Function prototypes
//
extern	hwTimer * hwTimers[];	// needed to resilve forward reference

hwTimer * hwTimerCreate 		(uint8 tmr, uint32 reload_val, uint8 mode, void (*expire_func)(struct _hwTimer *),
									uint32 prescale_val);
uint32 	hwTimerAttachCallback 	(hwTimer * t, void (*callback_func)(struct _hwTimer *));
uint32	hwTimerSetReLoadVal 	(hwTimer * t, uint32 new_reload_val);
uint32	hwTimerSetMaxVal 		(hwTimer * t, uint32 new_max_val);
uint32	hwTimerStart 			(hwTimer * t);
uint32	hwTimerRestart 			(hwTimer * t);
uint32	hwTimerStop 			(hwTimer * t);
uint32	hwTimerIsTiming			(hwTimer * t);

#endif /* TIMER_H_ */
