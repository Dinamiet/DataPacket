#ifndef _PACKET_STRUCTURE_H_
#define _PACKET_STRUCTURE_H_

#include "datapacket.h"

typedef struct _PacketHeader_
{
	uint8_t  MessageID;
	uint8_t  Length;
	uint16_t Checksum;
} PacketHeader;

typedef struct _Packet_
{
	PacketHeader Header;
	uint8_t      Data[DATAPACKET_MAX_SIZE - (sizeof(PacketHeader))];
} Packet;

#endif
