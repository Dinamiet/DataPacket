#ifndef _DATAPACKET_H_
#define _DATAPACKET_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DATAPACKET_MAX_SIZE 255

typedef struct _DataPacket_ DataPacket;

typedef size_t (*DataPacket_TransferFunction)(const void* data, const size_t size);
typedef void (*DataPacket_MessageFunction)(const DataPacket* dp, const void* data, const size_t size);

typedef struct _DataPacketMessage_
{
	uint8_t                    ID;
	DataPacket_MessageFunction Callback;
} DataPacketMessage;

typedef struct _DataPacket_
{
	DataPacket_TransferFunction Read;
	DataPacket_TransferFunction Write;
	const DataPacketMessage*    Messages;
	uint8_t                     Buffer[DATAPACKET_MAX_SIZE];
	size_t                      Size;
} DataPacket;

void DataPacket_Init(
		DataPacket*                       dp,
		const DataPacketMessage*          messages,
		const DataPacket_TransferFunction read_callback,
		const DataPacket_TransferFunction write_callback);

void DataPacket_Receive(DataPacket* dp);

void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size);

#endif
