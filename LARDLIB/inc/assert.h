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

void __assertion_failed(char *_Expr) __attribute__ ((noreturn));

#define USE_ERR_STRING 0
#define	FORCE	FALSE

void  __assert_handler(uint32 err, char * x) {

	if (eventFunctions[EVENT_SYS_ERROR] != NULL) {
		(eventFunctions[EVENT_SYS_ERROR]) ();
	} else {
		printf ("!%04x.%04x:%s\n",err >> 16, err & 0xFFFF, x);
	}

	sysErrRaise (err);
}

#define _ASSERT_STR(x) _ASSERT_VAL(x)
#define _ASSERT_VAL(x) #x

#define ASSERT(expr, err)   \
		((expr) ? (void)0 : __assert_handler(err, __FILE__ ":" _ASSERT_STR(__LINE__) ))

