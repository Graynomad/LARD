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
//////////////////////////////// error.h ////////////////////////////
//
//
//
//
//

#ifndef ERROR_H_
#define ERROR_H_


//(longjmp(syserr_env, _err))


#define CLEAR_RESULT	if (result != NULL) *result = 0;
#define SET_RESULT(r)	if (result != NULL) *result = r;

#define EXIT			goto exit;

void	syserrInit		(void);
void 	syserrRaise 	(uint32 err, char * x);
uint32	syserrGetCount	(void);
uint32	syserrGetLatest	(void);
void 	syserrDump		(void);
char * 	syserrGetLastErrStr ();
uint32 	syserrGetLastErr ();

extern	jmp_buf		syserr_env;			// not used at present

extern	void		__syserr_handler(uint32 err, char * x);
extern 	uint32		__last_syserr;

#define	FORCE		FALSE

////////////////////////////////////////////////////////////
//
// Structures should have guard bytes at the start and end.
// The byte at the start can be any value but should be a unique
// ID for that structure type (not tested for yet).
// The byte at the end should be the inverse of the start byte.
//
// This macro XORs the two, if the result is not 0 then one or
// other has been corrupted. This is considered to be a fatal fault.
//
// TODO: Have this also check the object type
//
#define VERIFY_OBJECT(s,id)								\
	if ((~(s->object_id ^ s->not_object_id) & 0xFF))	\
		FATAL(ERR_BAD_OBJECT | id);

#define FATAL(err) 		{syserrRaise ((uint32)err, "FATAL");}

#define SYS_ERROR(err)	{__sys_pc = __get_PC(); syserrRaise(err, "");}

#define TRY				(__last_syserr = NOERROR);

#define CATCH(err)		if (__last_syserr != NOERROR) \
								__syserr_handler(err, __FILE__ ":" _ASSERT_STR(__LINE__));

#define CATCH_RETERR(err)	if (__last_syserr != NOERROR) {									\
								__syserr_handler(err, __FILE__ ":" _ASSERT_STR(__LINE__));  \
								return err;}

#define CATCH_TO(x)		if (__last_syserr != NOERROR) x();

#define ASSERT(expr, err)  ((expr) ? (void)0 : \
		__syserr_handler(err, __FILE__ ":" _ASSERT_STR(__LINE__)))

#define	ASSERT_RETERR(expr, err) 										\
		if ((expr) == TRUE) {											\
			__syserr_handler(err, __FILE__ ":" _ASSERT_STR(__LINE__)); 	\
			return err;													\
		}

#define _ASSERT_STR(x) _ASSERT_VAL(x)
#define _ASSERT_VAL(x) #x

typedef enum  {

	ERR_NONE					= 0,
	ERR_TOO_MANY				= (0x01 << 16),
	ERR_INV_PIN					= (0x02 << 16),
	ERR_SETUP_FAILED			= (0x03 << 16),
	ERR_INV_INTERRUPT			= (0x04 << 16),
	ERR_BAD_MALLOC				= (0x05 << 16),
	ERR_SERIAL_BAD_DATA_BITS	= (0x06 << 16),
	ERR_SERIAL_BAD_STOP_BITS	= (0x07 << 16),
	ERR_SERIAL_BAD_PARITY		= (0x08 << 16),
	ERR_BUFFER_BAD_SIZE			= (0x09 << 16),
	ERR_MALLOC_FAILED 			= (0x0A << 16),
	ERR_SERIAL_INIT_FAILED		= (0x0B << 16),
	ERR_SPI_INIT_FAILED			= (0x0C << 16),
	ERR_BAD_PIN_FUNC			= (0x0D << 16),
	ERR_BUFFER_EMPTY			= (0x0E << 16),
	ERR_BUFFER_FULL				= (0x0F << 16),
	ERR_DIFF_PINPORT			= (0x10 << 16),
	ERR_INV_PINGROUP			= (0x11 << 16),
	ERR_VAL_TOO_LARGE			= (0x12 << 16),
	ERR_PINGROUP_NOT_DEFINED	= (0x13 << 16),
	ERR_BAD_ADC_CHANNEL			= (0x14 << 16),
	ERR_SERIAL_BAD_PORT			= (0x15 << 16),
	ERR_SPI_BAD_DATA_BITS		= (0x16 << 16),
	ERR_SPI_BAD_FRAME_FORMAT	= (0x17 << 16),
	ERR_BAD_OBJECT				= (0x18 << 16),
	BAD_DEBOUNCE_PERIOD			= (0x19 << 16),
	ERR_DEBOUNCE_INIT_FAILED	= (0x1A << 16),
	ERR_HWTIMER_BAD_RELOAD_VAL	= (0x1B << 16),
	ERR_HWTIMER_BAD_MODE		= (0x1C << 16),
	ERR_RESOURCE_CLASH			= (0x1D << 16),
	ERR_BAD_RESOURCE			= (0x1E << 16),
	ERR_TOO_MANY_SWTIMERS		= (0x1F << 16),
	ERR_SWTIMER_BAD_RELOAD_VAL	= (0x20 << 16),
	ERR_HEAPGUARD_FAILURE		= (0x21 << 16),
	ERR_CORRUPT_VARIABLE		= (0x22 << 16),
	ERR_STRING_ARRAY_TOO_LONG	= (0x23 << 16),
	ERR_PINGROUP_BAD_VAL		= (0x24 << 16),
	ERR_STRING_BAD_OFFSET		= (0x25 << 16),
	ERR_STRING_TRUNCATION		= (0x26 << 16),
	ERR_DPIN_HANDLER_NOT_SET	= (0x27 << 16),
	ERR_INV_VPIN				= (0x28 << 16),
	ERR_SERIAL_BAD_LOCATION		= (0x29 << 16),
	ERR_BAD_DELIMTER_LENGTH		= (0x30 << 16),
	ERR_HWTIMER_BAD_PRESCALE_VAL= (0x31 << 16),
	ERR_HWTIMER_BAD_MAX_VAL		= (0x32 << 16),
	ERR_SERIAL_BAD_CALLBACK_FUNCTION = (0x33 << 16),
	ERR_PACKET_INIT_FAILED		= (0x34 << 16),
	ERR_BAD_FSM_EVENT			= (0x35 << 16),
	ERR_BAD_FSM_STATE			= (0x36 << 16),
	ERR_BAD_FREE				= (0x37 << 16),
	ERR_TOO_MANY_SYSTICKS		= (0x38 << 16)

} syserr_types;

///////////////////////////////////////////////////////
//
//  Values used to qualify ERR_CORRUPT_VARIABLE
//
typedef enum {
	VAR_N_SWTIMERS = 1
} variable_err_types;

typedef enum {
	RSLT_BUFFER_EMPTY	= 1
} sysresult_types;


#endif /* ERROR_H_ */
