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
/////////////////////////////// string.c /////////////////////////////////
//
//  
//
//
//

#include "LARD.h"

/////////////////////////////////////////////////////////////////////
//
// Function name:		stringCreate
//
// Description:			Create a string object.
//
// Parameters:			uint32 size, the number of bytes required
//							for the string characters, not
//							including the NULL terminator.
//
// Returned value:		A pointer to the new string object or ERROR.
//
// Errors raised:		ERR_MALLOC_FAILED if the safeMalloc() call failed.
//
// Example:				string * str;
//						str = stringCreate(10);
//
//						Creates a string that houses 10 characters, for
//						example "0123456789" will fit because the function
//						allocates an extra byte for the NULL terminator.
//
// Notes:
//
string * stringCreate (uint32 size) {

	string * s;

	s = (void*)safeMalloc(sizeof (string));
	if (s == NULL) {
		SYS_ERROR (ERR_MALLOC_FAILED);
		return (string *)ERROR;
	}

	///////////////////////////////////////////////////
	// allocate 2 extra bytes, one for the null terminator
	// and one for a guard byte (currently not used)
	//
	s->str = (void*)safeMalloc(size + 2);
	if (s->str == NULL) {
		SYS_ERROR (ERR_MALLOC_FAILED);
		return (string *)ERROR;
	}

	s->object_id 	 = OBJID_STRING;
	s->not_object_id = ~OBJID_STRING;

	*(s->str + size) = GUARD_VAL;

	s->cur_len = 0;
	s->max_len = size;

	return s;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		stringLoadFromArray
//
// Description:			Load the string's internal array from an external
//							array of characters.
//
// Parameters:			string * s, pointer to a string object.
//						char * c, pointer to an array of char.
//
// Returned value:		ERROR if any errors were raised, NOERROR if not.
//
// Errors raised:		ERR_BAD_OBJECT if s is corrupted.
//						ERR_STRING_TRUNCATION if the source array's
//							NULL terminator was not encountered before
//							the copy filled the string's array.
//
// Example:				string * str;
//						str = stringCreate(10);
//						stringLoadFromArray (str, "0123456789");
//
//						The string now equals "0123456789", however
//
//						str = stringCreate(5);
//						stringLoadFromArray (str, "0123456789");
//
//						Causes an ERR_STRING_TRUNCATION error to be raised
//						and string to equal "01234"

// Notes:				In the case of a ERR_STRING_ARRAY_TOO_LONG error the
//						copy will be truncated.
//
uint32 stringLoadFromArray(string * s, char * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * to = s->str;
	uint8 * from = (uint8 *)c;
	uint32 len = s->max_len;

	while (len && *from) {
		*to++ = *from++;
		len--;
		s->cur_len++;
	}
	*to = NULLCHAR;

	if (len == 0 && (*from != 0)) {
		SYS_ERROR (ERR_STRING_ARRAY_TOO_LONG);
		return ERROR;
	}

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:		stringCopyToArray
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
uint32 stringCopyToArray(string * s, char * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * from = s->str;
	uint8 * to = (uint8 *)c;

	while (*from) {
		*to++ = *from++;
	}
	*to = NULLCHAR;

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringGetCharAt
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
uint8 stringGetCharAt(string * s, uint32 offset) {

	VERIFY_OBJECT(s, OBJID_STRING)

	if (offset >= s->max_len) {
		SYS_ERROR (ERR_STRING_BAD_OFFSET);
		return '?';
	}

	return (*(s->str + offset));

}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringCompare
//
// Description:		Compare two strings for equality.
//
// Parameters:		string * s, Pointer to a string object.
//					string * s1, Pointer to a string object.
//
// Returned value:	TRUE if the strings are equal, FALSE if not.
//
// Errors raised:	ERR_BAD_OBJECT if s or s1 are corrupted.
//
// Example:
//
// Notes:			Absolute case-sensitive char by char comparison.
//
boolean stringCompare(string * s, string * s1) {

	VERIFY_OBJECT(s, OBJID_STRING)
	VERIFY_OBJECT(s1, OBJID_STRING)

	if (s->cur_len != s1->cur_len)
		return FALSE;

	uint8 * sPtr = s->str;
	uint8 * s1Ptr = s1->str;
	uint32 len = s->cur_len;

	while (*sPtr) {
		if (*sPtr++ != *s1Ptr++)
			return FALSE;
		len--;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringEqualsArray
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
uint32 stringEqualsArray(string * s, char * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * sPtr = s->str;

	while (*sPtr) {
		if (*sPtr++ != *c++)
			return FALSE;
	}

	return *c == NULLCHAR ? TRUE : FALSE;
}

uint32 stringEqualsIgnoreCase(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringAppendA
//
// Description:		Append an external char array to a string.
//
// Parameters:		string * s, pointer to a string object.
//					char * c, pointer to an array of char.
//
// Returned value:	ERROR if any errors were raised, NOERROR if not.
//
// Errors raised:	ERR_BAD_OBJECT if s is corrupted.
//					ERR_STRING_TRUNCATION if the source array's
//						NULL terminator was not encountered before
//						the copy filled the string's array.
//
// Example:			string * str;
//					str = stringCreate(16);
//					stringLoadFromArray (str, "0123456789");
//					stringAppend(str, "ABCDEF");
//
//					The string now equals "0123456789ABCDEF", however
//
//					str = stringCreate(13);
//					stringLoadFromArray (str, "0123456789");
//					stringAppend(str, "ABCDEF");
//
//					Causes an ERR_STRING_TRUNCATION error to be raised
//					and string to equal "0123456789ABC"
//
// Notes:
//
uint32 stringAppend(string * s, char * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * to = s->str + s->cur_len;
	uint8 * from = (uint8 *)c;
	uint32 len = s->max_len - s->cur_len;

	while (len && *from) {
		*to++ = *from++;
		len--;
		s->cur_len++;
	}
	*to = NULLCHAR;

	if (len == 0 && (*from != 0)) {
		// We copied as many chars as possible but the
		// source array is still not at the end of the string
		SYS_ERROR (ERR_STRING_TRUNCATION);
		return ERROR;
	}
	return NOERROR;

}

uint32 stringEndsWith(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringStartsWith(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringGetBytes(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringIndexOf(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringLastIndexOf(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringLength(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringReplace(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringSetCharAt(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringSubstring(string * s, uint8 * c) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringToUpper(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	for (uint8 *p = s->str; *p; p++) {
		*p = toupper(*p);
	}

	return NOERROR;
}

uint32 stringToLower(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	for (uint8 *p = s->str; *p; p++) {
		*p = tolower(*p);
	}

	return NOERROR;
}

uint32 stringTrim(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringLeftTrim(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringRightTrim(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringTranslate
//
// Description:		Translate selected characters in a string.
//
// Parameters:		string * s, a pointer to a string object.
//					char * from, a pointer to an array of source chars to
//						search for.
//					char * to, a pointer to an array of replacement chars.
//
// Returned value:	ERROR if errors were encountered, NOERROR if not.
//
// Errors raised:	ERR_BAD_OBJECT if s is corrupted.
//
// Example:			string * str;
//					str = stringCreate(20);
//					stringLoadFromArray (str, ""%01---234/56789,abcdef"");
//					stringTranslate (str, "/%,-", "..~ ");
//
//					The string now equals ".01   234.56789~abcdef"
//
// Notes:			If any character in the "from" array is found in the string
//					it is replaced with the corresponding character (the one in
//					the same position) from the "to" array.
//
uint32 stringTranslate (string * s, char * from, char * to) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * ptr = s->str;
	uint8 * from_ptr;
	uint8 * to_ptr;

	while (*ptr) {
		from_ptr = (uint8 *)from;
		to_ptr = (uint8 *)to;
		while (*from_ptr) {
			if (*ptr == *from_ptr) {
				*ptr = *to_ptr;
			}
			from_ptr++;
			to_ptr++;
		}
		ptr++;
	}

	return NOERROR;
}

void _find_field (string * s, char delim, int8 field) {

	uint8 * ptr;

	switch (field) {
		case 0:
			ptr = s->str;			// start new search from beginning of string
			break;

		case -1:
			ptr = s->field_end + 1;	// start new search from the last one
			break;

		default:
			break;

	}

	s->field_start = ptr;

	while (*ptr != delim && *ptr != NULLCHAR) ptr++;

	s->field_end = ptr;

}

#if 1
uint32 stringGetField(string * s, uint8 field, char * delims, char * buf) {

	VERIFY_OBJECT(s, OBJID_STRING)

	uint8 * ptr = s->str;
//	uint8 * start = s->str, end;

	for (int i = 0; i < field; i++) {
		///////////////////////////////////////////////////////////
		// Find the first SOF delimiter
		while (*ptr != *delims && *ptr != NULLCHAR) ptr++;
		s->field_start = ++ptr;

		///////////////////////////////////////////////////////////
		// Search for the EOF delimiter, but if we find any more
		// SOF delimiters we reset the start of field pointer
		while (*ptr != *(delims+1) && *ptr != NULLCHAR) {
			if (*ptr == *delims) {
				s->field_start = ptr+1;
			}
			ptr++;
		}
		s->field_end = ptr;
	}

	///////////////////////////////////////////////////////////
	//
	// Move field into caller's array
	ptr = s->field_start;
	while (ptr != s->field_end) {
		*buf++ = *ptr++;
	}
	*buf = NULLCHAR;

	return NOERROR;

}
#else
uint32 stringGetField(string * s, uint8 field, char * delims, char * buf) {

	VERIFY_OBJECT(s, OBJID_STRING)

	ASSERT_RETERR (strlen(delims) <= field, ERR_BAD_DELIMTER_LENGTH);

	char * dptr = delims;
	uint8 * ptr;

	_find_field(s, *dptr++, 0);

	if (field > 0) {
		_find_field(s, *dptr++, -1);
	}

	ptr = s->field_start;
	while (ptr != s->field_end) {
		*buf++ = *ptr++;
	}
	*buf = NULLCHAR;

	return NOERROR;

}
#endif

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringGetFirstField
//
// Description:		Get the first field from a string based on a
//					caller-supplied delimiter.
//
// Parameters:		string * s, a pointer to a string object.
//					char * delim, the field delimiter.
//					char * buf, a pointer to an array of chars into
//						which the field should be copied.
//
// Returned value:	NOERROR.
//
// Errors raised:	ERR_BAD_OBJECT if s is corrupted. (This is FATAL)
//
// Example:			string * str;
//					char nmea_field[30];
//					str = stringCreate(20);
//					stringLoadFromArray (str, "$GPGGA,092204.999,4250.5589,S,14718.5084,E...");
//					stringGetFirstField(str, ',', nmea_field);
//
//					The variable nmea_field now holds the string "$GPGGA".
//
// Notes:			This function sets internal string variables so that
//					subsequent calls to stringGetNextField() know where to
//					start.
//
uint32 stringGetFirstField (string * s, char delim, char * buf) {

	VERIFY_OBJECT(s, OBJID_STRING)

	_find_field (s, delim, 0);

	char * ptr = (char *)s->field_start;

	while (ptr != (char *)s->field_end) {
		*buf++ = *ptr++;
	}
	*buf = NULLCHAR;

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//
// Function name:	stringGetNextField
//
// Description:		Get the next field from a string based on a
//					caller-supplied delimiter.
//
// Parameters:		string * s, a pointer to a string object.
//					char * delim, the field delimiter.
//					char * buf, a pointer to an array of chars into
//						which the field should be copied.
//
// Returned value:	NOERROR.
//
// Errors raised:	ERR_BAD_OBJECT if s is corrupted. (This is FATAL)
//
// Example:			string * str;
//					char nmea_field[30];
//					str = stringCreate(20);
//					stringLoadFromArray (str, "$GPGGA,092204.999,4250.5589,S,14718.5084,E...");
//					stringGetFirstField(str, ',', nmea_field);
//					//The variable nmea_field now holds the string "$GPGGA".
//
//					stringGetNextField(str, '.', nmea_field);
//					//The variable nmea_field now holds the string "092204".
//
//					stringGetNextField(str, ',', nmea_field);
//					//The variable nmea_field now holds the string "999".
//
//					Note how the first call to stringGetNextField() used the '.'
//					as a delimiter to grab the integer part of "092204.999", the
//					second call used ',' to grab the fractional part.
//
// Notes:			This function sets internal string variables so that
//					subsequent calls to stringGetNextField() know where to
//					start.
//
uint32 stringGetNextField(string * s, char delim, char * buf) {

	VERIFY_OBJECT(s, OBJID_STRING)

	_find_field (s, delim, -1);

	char * ptr = (char *)s->field_start;

	while (ptr != (char *)s->field_end) {
		*buf++ = *ptr++;
	}
	*buf = NULLCHAR;

	return NOERROR;

}



uint32 stringSetField(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}

uint32 stringRemoveField(string * s) {

	VERIFY_OBJECT(s, OBJID_STRING)

	return NOERROR;
}
