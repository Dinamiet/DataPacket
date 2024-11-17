#include "checksums.h"
#include "datapacket.h"
#include "packet_structure.h"

#include <assert.h>
#include <string.h>

void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size)
{
	assert(dp != NULL);
	assert(dp->Write != NULL);
	assert(messageID != 0); // Message ID of zero is reserved

	Packet packet;
	packet.Header.MessageID = messageID;
	packet.Header.Length    = sizeof(packet.Header);
	if (data)
	{
		packet.Header.Length += size;
		memcpy(packet.Data, data, size);
	}

	packet.Header.Checksum = CRC16(packet.Data, packet.Header.Length - sizeof(packet.Header), 0);
	packet.Header.Checksum = CRC16(&packet.Header, sizeof(packet.Header) - sizeof(packet.Header.Checksum), packet.Header.Checksum);
	packet.Header.Checksum = BIG_ENDIAN_16(packet.Header.Checksum);

	dp->Write(&packet, size + sizeof(packet.Header));
}
