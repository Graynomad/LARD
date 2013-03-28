////////////////////////// debug.c //////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

////////////////////////////////////////////////////////////////
//
// Function name:		debugPrintVar
//
// Description:
//
// Parameters:
//
// Returned value:
//
// Errors raised:
//
// Example:
//
// Notes:
//
// TODO:				Should not use any system functions to
//						dump the data. Move a copy of the shiftOut()
//						code into here and hard code so as not to
//						use any pinXxx() functions.
//
void debugPrintVar (uint32 var) {

	shiftOut32 (DBG_DATA_PIN, DBG_CLK_PIN, LSBFIRST, var);

}
