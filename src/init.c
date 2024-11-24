#include "datapacket.h"

#include <assert.h>

void DataPacket_Init(DataPacket* dp, const DataPacketMessage* messages, const DataPacket_ReadInterface read_interface, const DataPacket_WriteInterface write_interface)
{
	assert(dp != NULL);
	assert(messages != NULL);
	assert(read_interface != NULL);
	assert(write_interface != NULL);

	dp->Read     = read_interface;
	dp->Write    = write_interface;
	dp->Messages = messages;
	dp->Size     = 0;
}
