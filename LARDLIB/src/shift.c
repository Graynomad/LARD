////////////////////////// shift.c //////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

void _shiftOut (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val, uint32 bits) {

	if (bitOrder == MSBFIRST) {
		val <<= (32 - bits);		// move active bits to top of word
		for (uint32 i = 0; i < bits; i++)  {
			pinWrite (dataPin, (val & 0x80000000));
			val <<= 1;
			pinHigh (clockPin);
			pinLow (clockPin);
		}
	} else {
		for (uint32 i = 0; i < bits; i++)  {
			pinWrite (dataPin, (val & 0x01));
			val >>= 1;
			pinHigh (clockPin);
			pinLow (clockPin);
		}
	}
}

void shiftOut32 (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val) {
	_shiftOut (dataPin, clockPin, bitOrder, val, 32);
}

void shiftOut16 (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val) {
	_shiftOut (dataPin, clockPin, bitOrder, val, 16);
}

void shiftOut8 (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val) {
	_shiftOut (dataPin, clockPin, bitOrder, val, 8);
}

void shiftOut (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val) {
	_shiftOut (dataPin, clockPin, bitOrder, val, 8);
}

void shiftOutN (uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val, uint32 bits) {
	_shiftOut (dataPin, clockPin, bitOrder, val, bits);
}

uint32 shiftIn(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 bits) {
	uint8 value = 0;

	for (uint32 i = 0; i < bits; ++i) {
		pinHigh (clockPin);
		if (bitOrder == LSBFIRST)
			value |= pinRead(dataPin) << i;
		else
			value |= pinRead(dataPin) << (7 - i);
		pinLow (clockPin);
	}
	return value;
}
