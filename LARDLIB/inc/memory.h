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
