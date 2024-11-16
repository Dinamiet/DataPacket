#ifndef _PACKET_STRUCTURE_H_
#define _PACKET_STRUCTURE_H_

#include "datapacket.h"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BIG_ENDIAN_16(x)    __builtin_bswap16(x)
#define LITTLE_ENDIAN_16(x) (x)
#define BIG_ENDIAN_32(x)    __builtin_bswap32(x)
#define LITTLE_ENDIAN_32(x) (x)
#else
#define BIG_ENDIAN_16(x)    (x)
#define LITTLE_ENDIAN_16(x) __builtin_bswap16(x)
#define BIG_ENDIAN_32(x)    (x)
#define LITTLE_ENDIAN_32(x) __builtin_bswap32(x)
#endif

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
