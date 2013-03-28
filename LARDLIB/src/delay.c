///////////////////////////// delay.c /////////////////////////////////
//
//  
//
//
//

#include	"lard.h"

void delayMicroseconds(uint32 d) {
}

void delayMilliseconds(uint32 d) {
	uint32 stop = __msTicks + d;

	while (__msTicks <= stop);
}

#if 0
void delayx(uint32 ms) {
	uint32 start = millis();

	while (ms > 0) {
		if ((micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}
#endif
