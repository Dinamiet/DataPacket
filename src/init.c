#include "datapacket.h"

void DataPacket_Init(
		DataPacket*                       dp,
		const DataPacketMessage*          messages,
		const DataPacket_TransferFunction read_callback,
		const DataPacket_TransferFunction write_callback)
{
	dp->Read     = read_callback;
	dp->Write    = write_callback;
	dp->Messages = messages;
	dp->Size     = 0;
}
