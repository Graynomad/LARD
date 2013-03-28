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
//////////////////////////////// memory.h ////////////////////////////
//
//
//
//
//

#ifndef MEMORY_H_
#define MEMORY_H_

void * 	safeMalloc 		(size_t size);
void 	safeFree 		(void* m);

void 	heapGuardInit	(void);
void 	heapGuardCheck	(void);
uint32 	freeRam			(void);
uint32	stackPointer	(void);

#define HEAPGUARD_SIZE 	100
#define	HEAPGUARD_VAL	0xAA
#define	GUARD_VAL		0xAA

extern uint32 	__heap_size;
extern uint8 * 	__heap_start;
extern uint8 * 	__heap_end;
extern uint32 	__sys_pc;

extern uint32 	__get_PC(void);

#define malloc  malloc_not_allowed_use_safeMalloc
#define realloc realloc_not_allowed
#define free  	free_not_allowed_use_safefree

#endif /* MEMORY_H_ */
