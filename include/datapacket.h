#ifndef _DATAPACKET_H_
#define _DATAPACKET_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct _DataPacket_ DataPacket;

typedef bool (*DataPacket_TransferFunction)(const void* data, const size_t size);
typedef void (*DataPacket_MessageFunction)(const DataPacket* dp, const void* data, const size_t size);

typedef struct _DataPacketMessage_
{
	uint8_t                    ID;
	DataPacket_MessageFunction Callback;
} DataPacketMessage;

typedef struct _DataPacket_
{
	const DataPacketMessage*    Messages;
	DataPacket_TransferFunction Write;
	DataPacket_TransferFunction Read;
} DataPacket;

void DataPacket_Init(
		DataPacket*                       dp,
		const DataPacketMessage*          messages,
		const DataPacket_TransferFunction read_callback,
		const DataPacket_TransferFunction write_callback);

void DataPacket_Receive(DataPacket* dp);

void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size);

#endif
