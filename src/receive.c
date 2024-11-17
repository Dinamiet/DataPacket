#include "checksums.h"
#include "datapacket.h"
#include "packet_structure.h"

#include <assert.h>
#include <string.h>

static Packet* findValidPacket(uint8_t* buffer, size_t size);
static void    handlePacket(DataPacket* dp, Packet* packet);
static void    removePacketFromBuffer(DataPacket* dp, Packet* packet);

static Packet* findValidPacket(uint8_t* buffer, size_t size)
{
	size_t  offset = 0;
	Packet* packet;

	if (size < sizeof(packet->Header))
		return NULL;

	while (offset < size - sizeof(packet->Header))
	{
		packet = (Packet*)(buffer + offset);

		// Check valid message ID
		if (!packet->Header.MessageID)
		{
			offset++;
			continue;
		}

		// Check if packet size within range
		// if not within range, increment offset and continue searching for next potential packet
		if (packet->Header.Length > DATAPACKET_MAX_SIZE - sizeof(packet->Header))
		{
			offset++;
			continue;
		}

		// Ensure there is enough data in the buffer to extract a full packet
		// if not enough data, increment offset and continue searching for next potential packet
		if (size < packet->Header.Length + sizeof(packet->Header))
		{
			offset++;
			continue;
		}

		// Check if the packet is valid by comparing the checksum
		uint16_t checksum = BIG_ENDIAN_16(CRC16(packet->Data, packet->Header.Length, 0));
		if (checksum != packet->Header.Checksum)
		{
			offset++;
			continue;
		}

		// Packet is valid, return it
		return packet;
	}

	return NULL;
}

static void handlePacket(DataPacket* dp, Packet* packet)
{
	const DataPacketMessage* message = dp->Messages;
	while (message->Callback)
	{
		if (message->ID == packet->Header.MessageID)
		{
			message->Callback(dp, packet->Data, packet->Header.Length);
			return;
		}

		message++;
	}
}

static void removePacketFromBuffer(DataPacket* dp, Packet* packet)
{
	size_t   packetBufferSize = sizeof(packet->Header) + packet->Header.Length;
	uint8_t* packetEnd        = ((uint8_t*)packet + packetBufferSize);
	dp->Size -= (uint8_t*)packet - dp->Buffer; // Remove bytes that does not form a valid packet
	dp->Size -= packetBufferSize;              // Remove bytes that form a valid packet

	memmove(dp->Buffer, packetEnd, dp->Size); // Move remaining bytes to start of buffer
}

void DataPacket_Receive(DataPacket* dp)
{
	assert(dp != NULL);
	assert(dp->Read != NULL);

	if (dp->Size >= DATAPACKET_MAX_SIZE) // Buffer is full - remove first byte and shift the rest left to make space for new data
	{
		memmove(dp->Buffer, &dp->Buffer[1], DATAPACKET_MAX_SIZE - 1);
		dp->Size--;
	}

	dp->Size += dp->Read(&dp->Buffer[dp->Size], DATAPACKET_MAX_SIZE - dp->Size);

	Packet* packet = findValidPacket(dp->Buffer, dp->Size);

	if (packet)
	{
		handlePacket(dp, packet);
		removePacketFromBuffer(dp, packet);
	}
}
