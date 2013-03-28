
#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <LARD.h>

void setup 	(void);
void loop 	(void);

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

void sysTickHandler (void) {

	pin14high();
	swTimerScan();

	/////////////////////////////////////////////////////
	// If the user is using debounced pins handle that
	if (__debounceInterval)
		__debounceReadPins();

	pin14low();
}

extern void _vStackTop(void);

//__attribute__( ( always_inline ) ) static __INLINE uint32_t __get_PC(void)
//{
//  register uint32_t result;
//
//  __ASM volatile ("MRS %0, pc\n"  : "=r" (result) );
//  return(result);
//}

void init () {

	// Enable AHB clock for GPIO ports
	LPC_SYSCON->SYSAHBCLKCTRL |= _BV(AHB_GPIO1) | _BV(AHB_GPIO0);

	SysTick_Config(SystemCoreClock / 1000);

	syserrInit();

	adcInit(9000000);	// TODO: ADC should only be initialised when a pin
						//			is set to be an analog input

	attachEventHandler(EVENT_SYS_TICK, sysTickHandler);

	LPC_GPIO0->IE = 0UL;	// make sure all pin interrupts are disabled
	LPC_GPIO1->IE = 0UL;

#if 0	// 45 Mhz setup from LPC forum

	LPC_SYSCON->SYSOSCCTRL = 0;
	LPC_SYSCON->PDRUNCFG &= ~(1 << 5);
	LPC_SYSCON->SYSOSCCTRL = 1; // <-- Problem !!!!
	for (uint32_t i = 0; i < 200; i++)

#define SYSOSCCTRL_Val 0x00000000 // <-- The problem is here when this constant is 1
#define __XTAL 15000000
#define SYSPLLCLKSEL_Val 0x00000001
#define SYSPLL_SETUP 1
#define SYSPLLCTRL_Val 0x00000022
#define MAINCLKSEL_Val 0x00000003
#endif


}

int main(void) {

	ATOMIC_START

//	uint32_t x = __get_PC();


		heapGuardInit();
		printf("Free RAM:%d\n", freeRam());

		__system_initialising = TRUE;
		init();
		setup();
		__system_initialising = FALSE;

		ASSERT (syserrGetCount() == 0, ERR_SETUP_FAILED | syserrGetCount());

		printf("Free RAM:%d\n", freeRam());

	ATOMIC_END

	syserrRaise (ERR_RESOURCE_CLASH, "Some error occured");

	while(1) {
		swTimerExecuteCallbacks();
		heapGuardCheck();
		loop();
	}

	return 0 ;
}
