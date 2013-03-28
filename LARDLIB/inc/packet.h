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

#ifndef PACKET_H_
#define PACKET_H_

typedef struct _packet Packet;
//typedef void (*serialFuncPtr)	(struct _serialConnection *, uint32 event);

struct _packet {

	uint8	object_id;

	serialConnection * 	serial;
	string			 *	str;

	uint8	not_object_id;

} ;


void 		packetSetBeginDelim 	(Packet * p, char d);
void 		packetSetDelimEnd 		(Packet * p, char d);
Packet * 	packetCreate 			(serialConnection * s, uint32 size, char begin, char end);
uint32		packetLoad 				(Packet * p, char * s);
uint32		packetSend 				(Packet * p);


#endif /* PACKET_H_ */
