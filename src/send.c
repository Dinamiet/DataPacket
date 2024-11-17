#include "checksums.h"
#include "datapacket.h"
#include "packet_structure.h"

#include <assert.h>
#include <string.h>

void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size)
{
	assert(dp != NULL);
	assert(dp->Write != NULL);

	Packet packet;
	uint16_t length = sizeof(packet.Header);
	if (data)
	{
		length += size;
		memcpy(packet.Data, data, size);
	}

	// Correct header fields endianness
	packet.Header.MessageID = BIG_ENDIAN_16(messageID);
	packet.Header.Length    = BIG_ENDIAN_16(length);

	uint16_t checksum      = CRC16(&packet.Header, sizeof(packet.Header) - sizeof(packet.Header.Checksum), 0);
	checksum               = CRC16(packet.Data, length - sizeof(packet.Header), checksum);
	packet.Header.Checksum = BIG_ENDIAN_16(checksum);

	dp->Write(&packet, length);
}
