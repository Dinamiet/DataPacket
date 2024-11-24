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
		uint16_t length = BIG_ENDIAN_16(packet->Header.Length);

		// Check if packet size within range
		// if not within range, increment offset and continue searching for next potential packet
		if (length == 0 || length > DATAPACKET_MAX_SIZE)
		{
			offset++;
			continue;
		}

		// Ensure there is enough data in the buffer to extract a full packet
		// if not enough data, increment offset and continue searching for next potential packet
		if (size < length)
		{
			offset++;
			continue;
		}

		// Check if the packet is valid by comparing the checksum
		uint16_t checksum = CRC16(&packet->Header, sizeof(packet->Header) - sizeof(packet->Header.Checksum), 0);
		checksum          = CRC16(packet->Data, length - sizeof(packet->Header), checksum);
		if (checksum != BIG_ENDIAN_16(packet->Header.Checksum))
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
	while (message->Handler)
	{
		if (message->ID == BIG_ENDIAN_16(packet->Header.MessageID))
		{
			message->Handler(dp, packet->Data, BIG_ENDIAN_16(packet->Header.Length) - sizeof(packet->Header));
			return;
		}

		message++;
	}
}

static void removePacketFromBuffer(DataPacket* dp, Packet* packet)
{
	uint8_t* packetEnd = ((uint8_t*)packet + BIG_ENDIAN_16(packet->Header.Length));
	dp->Size -= (uint8_t*)packet - dp->Buffer; // Remove bytes that does not form a valid packet
	dp->Size -= BIG_ENDIAN_16(packet->Header.Length); // Remove bytes that form a valid packet

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
