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
///////////////////////////// debounce.c /////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

//uint32 debouncePins[MAX_PINS];

/////////////////////////////////////////////////////////////
// Pointers to arrays of port readings, each __debounceInterval
// words in size
//
uint32 * debouncePinArrays[3];		// allow for up to 3 ports

/////////////////////////////////////////////////////////////
// Let's assume that the debounce time is set to 10mS, therefore
// the debouncePinArrays are 10 deep and filled with samples taken
// every mS for the last 10mS.
//
// In this example pin19 has returned a 1 for 6 of the last reads.
//
//                                     1    1    2    2    3
//                           0....5....0....5....0....5....0..
// debouncePinArrays[0][0] = 000000000000000000010000000000000
// debouncePinArrays[0][1] = 000000000000000000010000000000000
// debouncePinArrays[0][2] = 000000000000000000010000000000000
// debouncePinArrays[0][3] = 000000000000000000010000000000000
// debouncePinArrays[0][4] = 000000000000000000000000000000000
// debouncePinArrays[0][5] = 000000000000000000010000000000000
// debouncePinArrays[0][6] = 000000000000000000000000000000000
// debouncePinArrays[0][7] = 000000000000000000010000000000000
// debouncePinArrays[0][8] = 000000000000000000000000000000000
// debouncePinArrays[0][9] = 000000000000000000000000000000000
//
// Step 1:
// We need to extract that vertical data and transform it into
// a single word.
//
// 000000000000000000000001111010100
//
// Step 2:
// Compare that value against
//
// 2^__debounceInterval -1 (1023 DEC for a 10mS debounce)
//
// if the value is == then the input has been HIGH for 10mS
// if the value is 0  then the input has been LOW for 10mS
// any other value (as in this case) and it has not been stable
//   in the last 10mS
//

uint32 debounceReadPin (uint32 pin) {

	uint32 pinDebounceVal = 0;
	uint32 port = pinPort(pin);
	uint32 pin_pos = pinPos(pin);
	uint32 x;

	// Step 1
	// Extract the vertical data and transform it into
	// a single word.
	for (int i = 0; i < __debounceInterval; i++) {
		// get the port reading for that mS
		x = debouncePinArrays[port][i];

		// move the bit we need into the LSB
		x >>= pin_pos;

		// Isolate the bit
		x &= 1;

		// Add this bit to the accumulated word
		pinDebounceVal |= x;

		// shift accumulated word
		pinDebounceVal <<= 1;
	}

	// Step 2
	// And compare that value
	if (pinDebounceVal == 0)
		return LOW;

	if (pinDebounceVal == (pinDebounceVal ^ 2) -1)
		return HIGH;

	return -1;

}

uint32 debounceCreate (uint32 ms) {

	if (ms < 3 || ms > 50) {
		SYS_ERROR (BAD_DEBOUNCE_PERIOD);
		return ERROR;
	}

	/////////////////////////////////////////////////////////////////
	// Create the debounce arrays, one for each port
	debouncePinArrays[0] = (void*)safeMalloc(ms * 4);
	if (debouncePinArrays[0] == NULL) {
		SYS_ERROR (ERR_DEBOUNCE_INIT_FAILED | 1);
		return ERROR;
	}

	debouncePinArrays[1] = (void*)safeMalloc(ms * 4);
	if (debouncePinArrays[1] == NULL) {
		SYS_ERROR (ERR_DEBOUNCE_INIT_FAILED | 2);
		return ERROR;
	}

	/////////////////////////////////////////////////////////////////
	// Clear the arrays and set the global that indicates the debounce time
	memset (debouncePinArrays[0], 0, ms * 4);
	memset (debouncePinArrays[1], 0, ms * 4);
	__debounceInterval = ms;

	return NOERROR;
}

/////////////////////////////////////////////////////////////////
//
// This is a non-user function that is called by SysTick_Handler
// every mS, as such it is very tight and the work is done when and
// if the user ever wants to read a debounced pin.
//
void __debounceReadPins (void) {
	static uint8 index = 0;

	debouncePinArrays[0][index]   = LPC_GPIO0->PIN;
	debouncePinArrays[1][index++] = LPC_GPIO1->PIN;
	if (index >= __debounceInterval) index = 0;

}

#if 0
void __debounceReadPins_version1 (void) {

	uint32 portVal;

	portVal = LPC_GPIO0->PIN;
	for (int i = 0; i < PINS_ON_PORT0; i++) {
		debouncePins[i] <<= 1;
		debouncePins[i] |= (portVal & 1);
	}

	portVal = LPC_GPIO1->PIN;
	for (int i = PINS_ON_PORT0; i < PINS_ON_PORT0 + PINS_ON_PORT1; i++) {
		debouncePins[i] <<= 1;
		debouncePins[i] |= (portVal & 1);
	}
}
#endif
