///////////////////////////// memory.c ///////////////////////////////
//
//  
//
//
//

#include "LARD.h"
//#include "mem.h"
extern boolean system_initialising;
extern void *__end_of_heap;

uint32			__heap_size;
uint8 *			__heap_start;
uint32 			__free_ram;
uint32 			__sys_pc;


////////////////////////////////////////////////////////////////
//
// Function name:		safeMalloc
//
// Description:			Allocate memory but only if called from
//						within the setup() function.
//
// Parameters:			none
//
// Returned value:		NULL or a pointer to the allocated memory
//
// Errors raised:		ERR_BAD_MALLOC | 1, if not called from within setup()
//						ERR_BAD_MALLOC | 2, if the malloc() call failed
//
// Example:
//
// Notes:				This function needs to use malloc() but nobody
//						else should so we undef it for a moment then
//						redef it to produce an obvious error at compile time.
//
void * safeMalloc (size_t size) {

	void * x = 0;

#undef malloc

	if (__system_initialising) {
		x = malloc (size);
	} else {
		SYS_ERROR(ERR_BAD_MALLOC | 1);
	}

#define malloc  malloc_not_allowed_use_safemalloc

	if (x == NULL) {
		SYS_ERROR(ERR_BAD_MALLOC | 2);
	}

	__heap_size = (uint32)__end_of_heap - (uint32)__heap_start;
	__free_ram = (stackPointer() - (uint32)__end_of_heap) & 0x1FFF;

	return x;
}

void safeFree (void* m) {

#undef free

	if (__system_initialising) {
		free (m);
	} else {
		SYS_ERROR(ERR_BAD_FREE);
	}

#define free free_not_allowed_use_safefree
}

void heapGuardInit(void) {
	__heap_size = 0;
	__heap_start = __end_of_heap;
	__free_ram = (stackPointer() - (uint32)__end_of_heap) & 0x1FFF;
	memset (__end_of_heap, HEAPGUARD_VAL, __free_ram - 16);
}

void heapGuardCheck(void) {

	byte * ptr = __end_of_heap + 64; 	// +64? When using semi-hosted mode
										// printf seems to use some of the heap
										// 64 bytes appears to be enough to clear that

	for (int i = 0; i < HEAPGUARD_SIZE; i++) {
		if (*ptr++ != HEAPGUARD_VAL) {
			FATAL (ERR_HEAPGUARD_FAILURE | 2);
		}
	}
}


uint32 freeRam() {
	return __free_ram;
}

uint32 stackPointer(void) {
	return (uint32)__get_MSP();
}

//__attribute__( ( always_inline ) )  __INLINE uint32 __get_PC(void) {
uint32 __get_PC(void) {

  register uint32 result;

  __ASM volatile ("push	{lr}");
  __ASM volatile ("pop	{%0}"  : "=r" (result) );
//  __ASM volatile ("mov	%0,pc"  : "=r" (result) );

  return(result);
}



