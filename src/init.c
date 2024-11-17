#include "datapacket.h"

#include <assert.h>

void DataPacket_Init(
		DataPacket*                       dp,
		const DataPacketMessage*          messages,
		const DataPacket_TransferFunction read_callback,
		const DataPacket_TransferFunction write_callback)
{
	assert(dp != NULL);
	assert(messages != NULL);
	assert(read_callback != NULL);
	assert(write_callback != NULL);

	dp->Read     = read_callback;
	dp->Write    = write_callback;
	dp->Messages = messages;
	dp->Size     = 0;
}
