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
