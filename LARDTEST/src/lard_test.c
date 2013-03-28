
#include "LARD.h"

//#define	DBG_HWTIMERS
#define	DBG_SERIAL_TX
//#define	DBG_SERIAL_BURST
//#define	DBG_PACKET_TX
//#define	DBG_STRING
//#define	DBG_PINGROUP
//#define	DBG_FIFO
//#define	DBG_SHIFTOUT
//#define	DBG_PININT
//#define	DBG_SWTIMER
//#define	DBG_ADC
//#define	DBG_DELAY
//#define	DBG_ANALOG
//#define	DBG_FSM_PIN
//#define 	DBG_FSM_TEST
//#define	DBG_PULSE
//#define	DBG_DEBOUNCE
//#define	DBG_STRING


#define LED_PIN		7

//pinGroup * pg;

#ifdef 	DBG_SWTIMER

void LEDoff (swTimer *);
void LEDon (swTimer *);

swTimer * swt;
swTimer * swttimers[9];

void LEDon (swTimer * t) {

	digitalWrite (LED_PIN, HIGH);
	swTimerAttachCallback (swt, LEDoff);

}

void LEDoff (swTimer * t) {

	digitalWrite (LED_PIN, LOW);
	swTimerAttachCallback (swt, LEDon);

}
#endif

#ifdef DBG_FSM_PIN

Fsm *	fsm;
uint8	fsm_counter = 0;

#if 1

enum {
	EVENT_KEY,
	EVENT_KEY_HASH,
	EVENT_KEY_STAR,
	FSM_PIN_N_EVENTS
};

enum  {
	STATE_IDLE,		// waiting for key entry
	STATE_IN_PIN,	// accepting PIN numbers
	FSM_PIN_N_STATES
};


uint32	pin = 0;

uint32 handleKeyDummy (fsmEvent e) {
	printf ("D");
	return STATE_IDLE;
};

uint32 handleKey (fsmEvent e) {

	static uint32 keysEntered = 0;

	uint32 done = false;

	switch (e.val) {

		case '#':			// enter
			done = true;
			break;

		case '<':			// backspace
			printf ("<");
			keysEntered--;
			pin /= 10;
			return STATE_IN_PIN;

		case '*':			// clear
			pin = 0;
			keysEntered = 0;
			printf ("\n-reset\n");
			return STATE_IDLE;

		default:			// any number
			printf ("%d", e.val);
			pin *= 10;
			pin += e.val;
			done = (++keysEntered == 4);
			break;
	}

	if (done) {
		printf ("\nPIN = %04d\n\n", pin);
		pin = 0;
		keysEntered = 0;
		return STATE_IDLE;
	}

	return STATE_IN_PIN;
}

FSM_TABLE_BEGIN (keypad, FSM_PIN_N_STATES, FSM_PIN_N_EVENTS)

	FSM_STATE_BEGIN				// STATE_IDLE
		FSM_ACTION (handleKey)		// EVENT_KEY		STATE_IN_PIN/STATE_IDLE
		FSM_ACTION (NULL)			// EVENT_KEY_HASH	STATE_IDLE
		FSM_ACTION (NULL)			// EVENT_KEY_STAR	STATE_IDLE
	FSM_STATE_END

	FSM_STATE_BEGIN				// STATE_IN_PIN
		FSM_ACTION (handleKey)		// EVENT_KEY		STATE_IN_PIN/STATE_IDLE
		FSM_ACTION (handleKey) 		// EVENT_KEY_HASH	STATE_IDLE
		FSM_ACTION (handleKey) 		// EVENT_KEY_STAR	STATE_IN_PIN
	FSM_STATE_END

FSM_TABLE_END
#endif

#if DBG_FSM_TEST
enum {
	EVENT_DN,
	EVENT_UP,
	FSM_N_EVENTS
};

enum  {
	STATE_0,
	STATE_1,
	STATE_2,
	FSM_N_STATES
} ;

uint32 action_dn1 (void) {
	fsm_counter-=1;
	printf ("%d ", fsm_counter);
	return STATE_1;
}

uint32 action_up1 (void) {
	fsm_counter+=1;
	printf ("%d ", fsm_counter);
	return STATE_1;
}

uint32 action_dn2 (void) {
	fsm_counter-=2;
	printf ("%d ", fsm_counter);
	return STATE_2;
}

uint32 action_up2 (void) {
	fsm_counter+=2;
	printf ("%d ", fsm_counter);
	return STATE_2;
}

uint32 action_wrap (void) {
	fsm_counter = 0;
	printf ("%d ", fsm_counter);
	return STATE_0;
}

FSM_STATE_TABLE_BEGIN (actions)

	FSM_STATE_BEGIN					// STATE_0
		FSM_ACTION (action_dn1)		// EVENT_DN		STATE_1
		FSM_ACTION (action_up1)		// EVENT_UP		STATE_1
	FSM_STATE_END

	FSM_STATE_BEGIN					// STATE_1
		FSM_ACTION (action_dn2)		// EVENT_DN		STATE_2
		FSM_ACTION (action_up2)		// EVENT_UP		STATE_2
	FSM_STATE_END

	FSM_STATE_BEGIN					// STATE_2
		FSM_ACTION (action_wrap)	// EVENT_DN		STATE_0
		FSM_ACTION (action_wrap)	// EVENT_UP		STATE_0
	FSM_STATE_END

FSM_STATE_TABLE_END
#if 0
uint32FuncPtr actions [FSM_N_STATES+1][FSM_N_EVENTS] = {
//	 EVENT_DN		EVENT_UP
	{action_dn1,	action_up1,},	// STATE_0
	{action_dn2,	action_up2,},	// STATE_1
	{action_wrap,	action_wrap,},	// STATE_2
};
#endif
#endif

#if 0
char * scores [] = { "LUV", "15", "30", "40", "DUECE" };
uint8 score_1 = 0;
uint8 score_2 = 0;

FSM_STATE_TABLE_BEGIN (tennis)

FSM_STATE_BEGIN					// STATE_2
	FSM_ACTION (action_wrap)	// EVENT_DN
	FSM_ACTION (action_wrap)	// EVENT_UP
FSM_STATE_END

FSM_STATE_TABLE_END
#endif



#endif

#ifdef DBG_ANALOG
void hwtAnalog_func (hwTimer * t) {

}
#endif

#ifdef DBG_HWTIMERS
void hwTimerTest1 (struct _hwTimer * t);
void hwTimerTest (struct _hwTimer * t) {
	pinToggle (7);
//	hwTimerSetReLoadVal (t, (t->reload_val + 10) & t->max_val);
	hwTimerSetReLoadVal (t, 50);
	hwTimerAttachCallback(t, hwTimerTest1);
}

void hwTimerTest1 (struct _hwTimer * t) {
	pinToggle (7);
//	hwTimerSetReLoadVal (t, (t->reload_val + 10) & t->max_val);
	hwTimerSetReLoadVal (t, 500);
	hwTimerAttachCallback(t, hwTimerTest);

}
#endif

#if 0
uint32 myDpinHandler1 (uint32 pin, uint32 func, uint32 data1, uint32 data2) {
	printf ("myDpinHandler1\n" );
	return NOERROR;
}

uint32 myDpinHandler2 (uint32 pin, uint32 func, uint32 data1, uint32 data2) {
	printf ("myDpinHandler2\n" );
	return NOERROR;
}
#endif

#ifdef DBG_PACKET_TX

	serialConnection * s;
	Packet			 * p;

#endif

#ifdef DBG_SERIAL_TX

  serialConnection * s0;

    hwTimer * hwtS0;

	void s0breakHandler (hwTimer * t) {
		serialEndBreak (s0);
	}


    void serialEvenHandler (serialConnection * s, uint32 e) {

    }
#endif

#ifdef DBG_SERIAL_BURST

  serialConnection * s0;

#endif

#ifdef DBG_PULSE

    hwTimer * hwtS0;

	void timerCallback (hwTimer * t) {
		pin15low();
	}


#endif


void fifoFull_handler (fifo8 * f) {
	printf ("Fifo error\n");
}

void sysErrorHandler (void){
	printf ("!%04X.%04X %s\n", syserrGetLastErr() >> 16,
			syserrGetLastErr() & 0xFFFF, syserrGetLastErrStr());
}


//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// setup /////////////////////////////////////

void setup (void) {

// test array initialisation
//#define FIL_LEN 5
//static uint8_t Filter[FIL_LEN] = {[0 ... FIL_LEN-1] = 0};

pinMode(LED_PIN, OUTPUT);
pinMode(15, OUTPUT);
pinMode(14, OUTPUT);

attachEventHandler(EVENT_SYS_ERROR, sysErrorHandler);
attachEventHandler(EVENT_FIFO_FULL, (void*)fifoFull_handler);

//printf ("SystemCoreClock:%d\n", SystemCoreClock);

//double f;
//printf ("%d\n", sizeof(f));

#ifdef DBG_FSM_TEST

fsm = fsmCreate (FSM_N_STATES, FSM_N_EVENTS, 10, STATE_0, (uint32FuncPtr) actions);

#endif

#ifdef DBG_FSM_PIN

fsm = fsmCreate (FSM_PIN_N_STATES, FSM_PIN_N_EVENTS, 50, STATE_IDLE, (fsmFuncPtr*) keypad);

#endif

#ifdef DBG_PACKET_TX

s = serialCreate(SERIAL_UART0, 300000, UART_DATA_BITS_8,
							UART_PARITY_NONE, UART_STOP_BITS_1, 100, 100);

p = packetCreate (s, 100, '<', '>');

#endif

#ifdef DBG_FIFO

fifo16 * fifo = fifo16Create (20);

for (int i = 0; i < 22; i++) {
	fifo16Write (fifo, i);
}

#endif

#ifdef DBG_FIFO1
	TRY
		fifo32 * buf = fifo32Create(2);
		fifo32Write(buf, 1);
		fifo32Write(buf, 2);
		uint32 x = fifo32Read(buf);
		fifo32Write(buf, 3);
		x = fifo32Read(buf);
		if (RESULT != RSLT_BUFFER_EMPTY) ;
		x = fifo32Read(buf);
		x = fifo32Read(buf);
		x = fifo32Read(buf);
		x = fifo32Read(buf);
	CATCH
#endif


#ifdef	DBG_DELAY
	delayMilliseconds(10);
#endif

#ifdef	DBG_SHIFTOUT
	pinMode (2, OUTPUT);
#endif

#if 0
//	uint32_t stack_frame[1];
//	for (int i = 0; i < 20; i++) {
//		printf ("%08x\n", stack_frame[i]);
//	}
#endif

#if 0
int analogvalues[4];
char msg[10];

	for (int Channel = 0; Channel < 4; Channel++) {
	    analogvalues[Channel] = Channel + (Channel * 100);
	  }
	msg[0] = 'b';
	msg[1] = 'c';
	msg[2] = highByte (analogvalues[0]);
	msg[3] = lowByte (analogvalues[0]);
	msg[4] = highByte (analogvalues[1]);
	msg[5] = lowByte (analogvalues[1]);
	msg[6] = highByte (analogvalues[2]);
	msg[7] = lowByte (analogvalues[2]);
	msg[8] = highByte (analogvalues[3]);
	msg[9] = lowByte (analogvalues[3]);

	for (int x=0; x<10; x++){
	  printf("%02x\n", msg[x]);
	 }

    int analog1 = (msg[2]<<8) + msg[3];
    int analog2 = (msg[4]<<8) + msg[5];
    int analog3 = (msg[6]<<8) + msg[7];
    int analog4 = (msg[8]<<8) + msg[9];
#endif

#ifdef DBG_SERIAL_TX

    s0 = serialCreate(SERIAL_UART0, 100000, UART_DATA_BITS_8,
								UART_PARITY_NONE, UART_STOP_BITS_1, 0, 150);

	serialAttachCallback (s0, UART_CALLBACK_CHARS_AVAILABLE, serialEvenHandler);

    hwtS0 = hwTimerCreate (HWTIMER_3, s0->break_val - 5, HWTIMER_MODE_MONOSTABLE,
 								s0breakHandler, HWTIMER_COUNT_RATE_US);

//   serialSendBreak (s0);

//	serialSetUartPins (s0, UART0_PINS_R40_T41);
//	serialLoopbackOn (s0);

//	serialStartBreak (s0);
//	delayMilliseconds(100);
//	serialEndBreak (s0);

#endif

#ifdef DBG_SERIAL_BURST
    s0 = serialCreate(SERIAL_UART0, 500000, UART_DATA_BITS_8,
								UART_PARITY_NONE, UART_STOP_BITS_1, 20, 0);

#endif

#ifdef DBG_HWTIMERS
	////////////////////////////////////// hardware timers ////////////////////////////////
	hwTimer * hwt;

	hwt = hwTimerCreate (HWTIMER_2, 1000UL, HWTIMER_MODE_ASTABLE, hwTimerTest, HWTIMER_COUNT_RATE_MS);
	hwTimerStart (hwt);
	hwTimerSetMaxVal (hwt, 255);
	hwTimerSetReLoadVal (hwt, 1000);
#endif

#ifdef DBG_PULSE

#if 0
	hwtS0 = hwTimerCreate (HWTIMER_3, 100, HWTIMER_MODE_MONOSTABLE,
    		timerCallback, HWTIMER_COUNT_RATE_US);
#else
    hwtS0 = hwTimerCreate (HWTIMER_3, 100, HWTIMER_MODE_MONOSTABLE,
    		NULL, HWTIMER_COUNT_RATE_US);
#endif

#endif
#ifdef DBG_PININT
//	attachEventHandler(EVENT_PININT_0, yy);
#endif

#ifdef DBG_PINGROUP
	pg = pinGroupCreate(OUTPUT, 0, 10);
	pinGroupWrite(pg, 0xaa);
#endif

#if DBG_VPINS
	enableVpinHandlers (40, 49, myDpinHandler1);
	attachVpinHandler (45, myDpinHandler2);
	attachVpinHandler (39, myDpinHandler2);
	attachVpinHandler (50, myDpinHandler2);
//	pinHigh(40);
#endif

#ifdef DBG_DEBOUNCE
	debounceCreate (20);
#endif

#ifdef DBG_ANALOG
	pinSetAsAnalogInput(A0);

	pinSetAsDigitalInput(3);
	pinSetAsDigitalInput(3);
	pinSetAsDigitalInput(3);

	pinGroupInc(pg);

	hwTimer * hwt;
	hwt = hwTimerCreate(0, 1000, HWTIMER_MODE_ASTABLE, hwtAnalog_func);
#endif

#if 0
//	uint32 y = _pvHeapStart;
//	uint32 y = __get_MSP();
//	__check_heap_overflow ();
//	__WFI();
#endif

#ifdef DBG_STRING

	string * str2;
	string * str1;

	str2 = stringCreate(100);
	str1 = stringCreate(20);

	char s [100];
	char * nmea  = "$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F";
	stringLoadFromArray (str2, nmea);
	stringGetFirstField(str2, ',', s);
	printf("%s\n", s);					// $GPGGA
	stringGetNextField(str2, '.', s);
	printf("%s\n", s);					// 092204
	stringGetNextField(str2, ',', s);
	printf("%s\n", s);					// 999
	stringGetField(str2, 2, ",.", s);
	printf("%s\n", s);					// 4250
	stringGetField(str2, 3, ",.", s);
	printf("%s\n", s);					// S,14718
	stringGetField(str2, 2, ",,", s);
	printf("%s\n", s);					// 4250.5589
	stringGetField(str2, 8, ",,", s);
	printf("%s\n", s);					// 24.4
	stringGetField(str2, 1, ",*", s);
	printf("%s\n", s);					// 0000
	stringGetField(str2, 1, "*x", s);
	printf("%s\n", s);					// 1F

	stringLoadFromArray (str2, "!001d.0001:../src/resources.c:83");
	stringGetFirstField(str2, '!', s);
	stringGetNextField(str2, ':', s);
	printf("%s\n", s);					// 001d.0001
	stringGetNextField(str2, ':', s);
	printf("%s\n", s);					// ../src/resources.c
	stringGetNextField(str2, ':', s);
	printf("%s\n", s);					// 83

	stringTranslate (str2, ",0SEM", "|osem");

	stringLoadFromArray (str1, "1234567890ABCDEFGHIJ");

	printf("%s\n", stringEqualsArray(str2, "1234567890ABCDEFGHIJ") ? "T" : "F");
	printf("%s\n", stringEqualsArray(str2, "1234567890AbCDEFGHIJ") ? "T" : "F");

//	stringAppend (str, "abcdefghijk");
//	stringCopyToArray (str, x);

	string * str3;
	char my_char_array [100];
	char * nmea1  = "+CMGR: \"REC UNREAD\",\"+8613920001234\",\"\",\"12/12/13,15:08:55+50\"";
	str3 = stringCreate(100);
	stringLoadFromArray (str3, nmea1);

	stringGetField(str3, 2, "+\"", my_char_array);
	printf("%s\n", s);					// printd 8613920001234

#endif

#ifdef	DBG_ADC
	uint16 x = adcRead(A0);
#endif

#ifdef 	DBG_SWTIMER
	swt = swTimerCreate (30, 0, SWTIMER_TYPE_ASTABLE, LEDon);
	swTimerStart(swt);
	for (int i = 0; i < 9; i++) {
		swttimers[i] = swTimerCreate (30, 0, SWTIMER_TYPE_ASTABLE, NULL);
		swTimerStart(swttimers[i]);
	}

#endif

#if 0
//	uint32 start = SysTick->VAL;
//	pinToggle(LED_PIN);
////	__NOP();
//	uint32 stop = SysTick->VAL;
//	printf ("%d\n%d\n", start, stop);
#endif

}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// loop /////////////////////////////////////
void loop (void) {

#ifdef	DBG_SHIFTOUT
	shiftOutN (2, 7, MSBFIRST, 0x123, 12);
	delayMilliseconds(1);
#endif

#ifdef DBG_PACKET_TX

//	packetLoad(p, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	packetLoad(p, "ABCDEFGHIJKLM");
	packetSend(p);

	serialWaitForTxToClear(s);
	delayMilliseconds(1000);

#endif

#ifdef DBG_SERIAL_BURST

//	for (int i = 0; i < 16; i++) {
//		serialFifoWrite (s0, i + 'A');
//	}

	pin7high();
	serialPrintString (s0, "0123456789ABCDEF");
	pin7low();

	delayMilliseconds(1000);

#endif

#ifdef DBG_SERIAL_TX

//	serialStartBreak (s0);
//	hwTimerStart (hwtS0);
//	while (hwtS0->state == HWTIMER_STATE_TIMING) ;

// serialSendBreak(s0);

//	serialPrintString (s0, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
//	serialPrintString (s0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//	serialPrintString (s0, "ABCDEFGHIJKLMNOP");

	for (int i = 1; i < 101; i++) {
		serialWrite (s0, i);
	}

//	serialWaitForTxToClear(s0);

//	while (serialAvailable(s0) < 16) ;
//	printf ("\n");
//	while (serialAvailable(s0)) {
//		printf ("%c", serialRead (s0) & 0xff);
//		serialRead (s0);
//	}

	delayMilliseconds(1000);

//	while (millis() % 1000 != 0);

#endif

#ifdef DBG_PULSE

#if 0
	for (uint32 i = 10; i <= 100; i+= 10) {
		hwTimerSetReLoadVal (hwtS0, i -5);
		pin15high();
		hwTimerStart (hwtS0);
		while (hwtS0->state == HWTIMER_STATE_TIMING) ;
	}
#else
	for (uint32 i = 10; i <= 100; i+= 10) {
		hwTimerSetReLoadVal (hwtS0, i-6);
		pin15high();
		hwTimerStart (hwtS0);
		while (hwtS0->state == HWTIMER_STATE_TIMING) ;
		pin15low();
	}
#endif


	delayMilliseconds(1000);

#endif


#if 0
	unsigned long input_number=1922222222, i, x=1;

	digitalWrite(2, LOW);
	for (int i = 0; i < 5; i++) {
		x = input_number % 10;
		x <<= 4;
		input_number /= 10;
		x |= input_number % 10;
		input_number /= 10;
		shiftOut(4, 3,MSBFIRST, x);
		printf ("%x ", x);
//		Serial.print (x, HEX);
	}
	digitalWrite(2, LOW);
//	Serial.println ("");



#endif

#if 0
//	float x = 3.0 / 2.0;
//	printf ("------%1.1f\n", x);

//	uint32 * xxx =  & LPC_GPIO0->NOT;

//	static uint32 start;
//	static uint32 stop;
//
//	start = msTicks;
//	for (int i = 0; i < 1000000; i++) {
//		pin7Toggle();
//	}
//	stop = msTicks;
//	printf ("Time:%d\n", stop - start);

#endif


#ifdef DBG_FSM_TEST
	fsmRaiseEvent (fsm, EVENT_UP);
	fsmRaiseEvent (fsm, EVENT_UP);
	fsmRaiseEvent (fsm, EVENT_UP);
	fsmRaiseEvent (fsm, EVENT_UP);

	fsmHandleEvents(fsm);

	printf ("\n");

	delayMilliseconds(1000);
#endif


#ifdef DBG_FSM_PIN

	fsmRaiseEvent (fsm, EVENT_KEY, 1);
	fsmRaiseEvent (fsm, EVENT_KEY, 2);
	fsmRaiseEvent (fsm, EVENT_KEY, 3);
	fsmRaiseEvent (fsm, EVENT_KEY_STAR, '*');
	fsmRaiseEvent (fsm, EVENT_KEY, 4);
	fsmRaiseEvent (fsm, EVENT_KEY, 5);
	fsmRaiseEvent (fsm, EVENT_KEY, 6);
	fsmRaiseEvent (fsm, EVENT_KEY, '<');
	fsmRaiseEvent (fsm, EVENT_KEY, '<');
	fsmRaiseEvent (fsm, EVENT_KEY, 1);
	fsmRaiseEvent (fsm, EVENT_KEY, 2);
	fsmRaiseEvent (fsm, EVENT_KEY, 3);

	fsmRaiseEvent (fsm, EVENT_KEY_HASH, '#');

	fsmHandleEvents(fsm);

	printf ("\n");

	delayMilliseconds(1000);
#endif



#ifdef DBG_PINGROUP
	pinGroupRotateRight(pg);
#endif

#if 0
//	static int counter = 0;
//	if (counter++ & 1) {
//		pinHigh(7);
//	} else {
//		pin7low();
//	}
//	delayMilliseconds(1000);
//	printf ("%d\n", msTicks);
//	uint32 xx = pinReadDebounced (1);
//	uint16 x = adcRead(A0);
//	printf ("%d\n", x);
#endif

}
