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

/**
 * Packet header structure
 */
typedef struct _PacketHeader_
{
	uint8_t  MessageID; /**< Packet message identifier */
	uint8_t  Length;    /**< Lenght of packet data, length does not include header size */
	uint16_t Checksum;  /**< Checksum of packet data, checksum does not include header information */
} PacketHeader;

/**
 * Packet structure
 */
typedef struct _Packet_
{
	PacketHeader Header;                                             /** Packet header inforamtion */
	uint8_t      Data[DATAPACKET_MAX_SIZE - (sizeof(PacketHeader))]; /** Packet data */
} Packet;

#endif
