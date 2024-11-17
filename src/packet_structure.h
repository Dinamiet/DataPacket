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
 * \note Fields are in big endian format
 */
typedef struct _PacketHeader_
{
	uint16_t MessageID; /**< Packet message identifier */
	uint16_t Length;    /**< Lenght of packet data, length includes packet and data size */
	uint16_t Checksum;  /**< Checksum of packet, checksum does includes header information other than checksum field itself */
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
