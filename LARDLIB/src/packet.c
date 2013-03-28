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
///////////////////////////// packet.c /////////////////////////////////
//
//  
//
//
//

#include	"lard.h"

void __packetSetDelimBegin (serialConnection * s, char d);
void __serialSetDelimEnd (serialConnection * s, char d);

INLINE void packetSetBeginDelim (Packet * p, char d) {

	VERIFY_OBJECT (p, OBJID_ASCII_PACKET);

	__packetSetDelimBegin (p->serial, d);

}

INLINE void packetSetDelimEnd (Packet * p, char d) {

	VERIFY_OBJECT (p, OBJID_ASCII_PACKET);

	__serialSetDelimEnd (p->serial, d);

}

uint32 packetLoad (Packet * p, char * s) {

	stringLoadFromArray (p->str, s);

	return NOERROR;
}

uint32 packetSend (Packet * p) {

	serialWrite(p->serial, p->serial->delimBegin);
	serialPrintString (p->serial, (char*)p->str->str);
	serialWrite(p->serial, p->serial->delimEnd);
	return NOERROR;
}

Packet * packetCreate (serialConnection * s, uint32 size, char begin, char end) {

	Packet * p;

	VERIFY_OBJECT (s, OBJID_ASCII_PACKET);


	/////////////////////////////////////////////////////////////////
	// Create the packet structure
	p = (void*)safeMalloc(sizeof (Packet));

	if (p == NULL) {
		SYS_ERROR (ERR_PACKET_INIT_FAILED);
		return (Packet *)ERROR;
	}

	/////////////////////////////////////////////////////////////////
	// Create a string as a workspace for the packet
	p->str = stringCreate(size);

	if (p->str == (string*)ERROR) {
		SYS_ERROR (ERR_PACKET_INIT_FAILED);
		safeFree (p);
		return (Packet *)ERROR;
	}

	p->serial		 = s;
	p->object_id 	 = OBJID_ASCII_PACKET;
	p->not_object_id = ~OBJID_ASCII_PACKET;


	return p;
}
