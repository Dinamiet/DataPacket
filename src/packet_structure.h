#ifndef _PACKET_STRUCTURE_H_
#define _PACKET_STRUCTURE_H_

#include "datapacket.h"

typedef struct _PacketHeader_
{
	uint8_t  MessageID;
	uint8_t  Length;   /**< Only data, length does not include header size */
	uint16_t Checksum; /**< Only data, checksum does not include header information */
} PacketHeader;

typedef struct _Packet_
{
	PacketHeader Header;
	uint8_t      Data[DATAPACKET_MAX_SIZE - (sizeof(PacketHeader))];
} Packet;

#endif
