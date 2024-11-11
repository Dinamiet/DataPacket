#include "datapacket.h"
#include "packet_structure.h"

#include <string.h>

void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size)
{
	Packet packet;
	packet.Header.MessageID = messageID;
	packet.Header.Length    = size;
	packet.Header.Checksum  = 0; /** TODO: Generate checksum from data */
	memcpy(packet.Data, data, size);

	dp->Write(&packet, size + sizeof(packet.Header));
}
