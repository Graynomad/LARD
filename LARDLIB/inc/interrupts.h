//////////////////////////////// interrupts.h ////////////////////////////
//
//
//
//
//

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

//extern voidFuncPtr 		intFunctions[];
extern uint32	intStatus;
extern uint8	__interruptLevel;

__attribute__( ( always_inline ) ) static __INLINE void __lard_enable_irq(void) {
  __ASM volatile ("CPSIE i\n\t""dmb\n\t" ::: "memory");
}

__attribute__( ( always_inline ) ) static __INLINE void __lard_disable_irq(void) {
  __ASM volatile ("CPSID i\n\t""dmb\n\t" ::: "memory");
}


void forceFullInterruptScan (boolean x);

#define 	disableInterrupts()		ATOMIC_START
#define 	restoreInterrupts()		ATOMIC_END
//#define		ATOMIC_START			if (__interruptLevel){__lard_disable_irq();__DMB();__DSB();__ISB();}__interruptLevel++;
//#define		ATOMIC_END		if ((__interruptLevel) && !(--__interruptLevel))__lard_enable_irq();

#define		ATOMIC_START	__ASM volatile ("CPSIE i\n\t""dmb\n\t" ::: "memory"); 	\
							__DMB();__DSB();__ISB();								\
							if (__interruptLevel){__interruptLevel++;}

#define		ATOMIC_END		if ((__interruptLevel) && !(--__interruptLevel)) {		\
								__ASM volatile ("CPSID i\n\t""dmb\n\t" ::: "memory");}
#define		ATOMIC(x)							\
						ATOMIC_START			\
						x;						\
						ATOMIC_END

#endif /* INTERRUPTS_H_ */
