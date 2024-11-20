#include "datapacket.h"

#include <assert.h>

void DataPacket_Init(DataPacket* dp, const DataPacketMessage* messages, const DataPacket_ReadFunction read_interface, const DataPacket_WriteFunction write_interface)
{
	assert(dp != NULL);
	assert(messages != NULL);
	assert(read_callback != NULL);
	assert(write_callback != NULL);

	dp->Read     = read_interface;
	dp->Write    = write_interface;
	dp->Messages = messages;
	dp->Size     = 0;
}
