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
///////////////////////////// arduino.c /////////////////////////////
//
//  
//
//
//

#include "LARD.h"

////////////////////////////////////////////////////////////////////////////
//
// These point to structures used by the Arduino-compatible serial functions
//
serialConnection * arduinoSerial;
serialConnection * arduinoSerial1;

void attachInterrupt (uint8_t pin, void (*func)(void), int mode) {

	switch (mode) {

		case LOW:
			pinSetInterruptMode(pin, PININT_LOW_LEVEL);
			break;

		case CHANGE:
			pinSetInterruptMode(pin, PININT_BOTH_EDGES);
			break;

		case FALLING:
			pinSetInterruptMode(pin, PININT_FALLING_EDGE);
			break;

		case RISING:
			pinSetInterruptMode(pin, PININT_RISING_EDGE);
			break;

	}

	attachEventHandler(EVENT_PININT_0 + pin, func);
	pinEnableInterrupt(pin);

}

void detachInterrupt	(uint8_t pin) {

	pinDisableInterrupt(pin);
	detachEventHandler(EVENT_PININT_0 + pin);

}

void pinMode(uint8_t pin, uint8_t mode) {

	pinConfig ((uint32) pin, (uint32) mode);

}

////////////////////////////////////////////////////////////////
//
// The whole issue of Arduino compatibility needs to be thought about
// because we're using C here and Arduino uses C++
//
void Serial_begin (long baudrate) {
	arduinoSerial =  serialCreate(0, (uint32)baudrate, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1, 64, 64);
}
void Serial1_begin (long baudrate) {
	arduinoSerial1 =  serialCreate(1, (uint32)baudrate, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1, 64, 64);
}

uint32 Serial_available () {
	return serialAvailable (arduinoSerial);
}

uint32 Serial1_available () {
	return serialAvailable (arduinoSerial1);
}

uint32 Serial_write (uint8 b) {
	return serialWrite (arduinoSerial, b);
}

uint32 Serial1_write (uint8 b) {
	return serialWrite (arduinoSerial1, b);
}

uint32 Serial_flush () {
	return 1;

}

uint32 Serial_peek () {
	return 1;

}

uint32 Serial_read () {
	return 1;
}

uint32 Serial_print () {
	return 1;

}

uint32 Serial_println () {
	return 1;
}

INLINE uint16 analogRead(uint8 pin) {
	return adcRead((uint8_t)pin);
}

INLINE uint32 millis(void) {
	return msTicks;
}

uint32 micros (void){
	return SysTick->VAL;	// not right, needs to be looked into
}



