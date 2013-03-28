
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
