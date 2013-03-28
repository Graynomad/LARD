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
//////////////////////////////// lstring.h ////////////////////////////
//
//
//
//
//

#ifndef LSTRING_H_
#define LSTRING_H_

typedef		uint8_t 	uint8;
typedef		uint32_t	uint32;
typedef		uint8_t 	boolean;


typedef struct  {

	uint8	object_id;

	uint8 * str;
	uint8 * field_start;
	uint8 * field_end;

	uint32	cur_len;
	uint32	max_len;

	uint8	not_object_id;

} string;

string * 	stringCreate 				(uint32 size) ;
uint32 		stringLoadFromArray			(string * s, char * c);
uint32 		stringCopyToArray			(string * s, char * c);
uint8 		stringGetCharAt				(string * s, uint32 offset);
boolean 	stringCompare				(string * s, string * s1);
uint32 		stringEqualsArray			(string * s, char * c);
uint32 		stringAppend				(string * s, char * c);
uint32 		stringTranslate 			(string * s, char * from, char * to);

uint32		stringGetFirstField			(string * s, char delim, char * buf);
uint32 		stringGetNextField			(string * s, char delim, char * buf);
uint32 		stringGetField				(string * s, uint8 field, char * delims, char * buf);



#endif /* LSTRING_H_ */
