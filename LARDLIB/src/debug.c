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
