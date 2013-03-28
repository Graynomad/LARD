///////////////////////////// hardfault.c /////////////////////////////////
//
// A cut down version of the function from
//
// http://support.code-red-tech.com/CodeRedWiki/DebugHardFault
//
// This only grabs the LR which is all you need to see the address
// that caused the fault. Use the LR value to find the offending
// function in the MAP file.
//
// To use this you have to edit the cr_startup_lpc12xx.c file, change
// the 4th entry in the interrupt vector table from
//
//    HardFault_Handler, 	                  // The hard fault handler
//
// to
//
//    HardFault_HandlerAsm,                   // The hard fault handler
//
//

__attribute__((naked))
void HardFault_HandlerAsm(void){

        /*
         * Get the appropriate stack pointer, depending on our mode,
         * and use it as the parameter to the C handler. This function
         * will never return
         */

        __asm(  ".syntax unified\n"
                        "MOVS   R0, #4  \n"
                        "MOV    R1, LR  \n"
                        "TST    R0, R1  \n"
                        "BEQ    _MSP    \n"
                        "MRS    R0, PSP \n"
                        "B      HardFault_HandlerC      \n"
                "_MSP:  \n"
                        "MRS    R0, MSP \n"
                        "B      HardFault_HandlerC      \n"
                ".syntax divided\n") ;
}

void HardFault_HandlerC(unsigned long *hardfault_args){
        volatile unsigned long stacked_lr = 0;

        stacked_lr = ((unsigned long)hardfault_args[5]) ;

        __asm("BKPT #0\n") ; // Break into the debugger

}
