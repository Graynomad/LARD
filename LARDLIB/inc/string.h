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
