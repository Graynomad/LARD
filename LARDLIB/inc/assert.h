
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

