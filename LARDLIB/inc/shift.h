//////////////////////////////// shift.h ////////////////////////////
//
//
//
//
//

#ifndef SHIFT_H_
#define SHIFT_H_

void	shiftOut32	(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val);
void	shiftOut16	(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val);
void	shiftOut8	(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val);
void	shiftOutN	(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val, uint32 bits);
void	shiftOut	(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 val);
uint32	shiftIn		(uint32 dataPin, uint32 clockPin, uint32 bitOrder, uint32 bits);

#define LSBFIRST 0
#define MSBFIRST 1

#endif /* SHIFT_H_ */
