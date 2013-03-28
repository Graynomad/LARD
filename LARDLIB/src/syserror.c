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
//////////////////////////////// syserror.c ////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

static	uint32 		sysErrors [20];
static	uint32 		*sysErrorPtr = sysErrors;
static	uint32 		nSysErrors = 0;
static	char 		__last_syserr_str[100] = "";
		uint32		__last_syserr;


const uint32 MAX_SYS_ERRORS = (sizeof(sysErrors) / sizeof(sysErrors[0]));

jmp_buf syserr_env;
//		longjmp(syserr_env, err);

////////////////////////////////////////////////////////////////
//
// Function name:		sysErrRaise
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
void __syserr_handler (uint32 err, char * x) {

	if (nSysErrors >= MAX_SYS_ERRORS || strcmp (x, "FATAL") == 0) {
		fatal();
		// TODO: set all pins to inputs?
	}

	*sysErrorPtr++ = err;
	nSysErrors++;
	__last_syserr = err;
	strcpy (__last_syserr_str, x);

	if (eventFunctions[EVENT_SYS_ERROR] != NULL) {
		(eventFunctions[EVENT_SYS_ERROR]) ();
	}
}

void  syserrRaise(uint32 err, char * x) {

	__syserr_handler (err, x);

}

char * syserrGetLastErrStr () {

	return __last_syserr_str;

}

uint32 syserrGetLastErr () {

	return __last_syserr;

}

////////////////////////////////////////////////////////////////
//
// Function name:		sysErrInit
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
void syserrInit () {
	sysErrorPtr = sysErrors;
	nSysErrors = 0;
}

////////////////////////////////////////////////////////////////
//
// Function name:		sysErrGetCount
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
uint32 syserrGetCount () {

	return nSysErrors;

}

////////////////////////////////////////////////////////////////
//
// Function name:		sysErrPeek
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
uint32 syserrPeek () {

	if (nSysErrors == 0) return (ERR_NONE);

	return (*(sysErrorPtr-1));
}

////////////////////////////////////////////////////////////////
//
// Function name:		sysErrRead
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
uint32 syserrRead () {

	if (nSysErrors == 0) return (ERR_NONE);

	sysErrorPtr--;
	return (*(sysErrorPtr));

}


////////////////////////////////////////////////////////////////
//
// Function name:		sysErrDump
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
//
void syserrDump () {

	uint32 * errPtr = sysErrorPtr -1;

	for (int i = 0; errPtr >= sysErrors; i++) {
		debugPrintVar (*errPtr--);
	}

}
