#ifndef _DATAPACKET_H_
#define _DATAPACKET_H_

/**
 * \file
 * Data Packet
 *
 * A simple binary message transfer protocol
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DATAPACKET_MAX_SIZE 256 /**< Maximum size of a data packet in bytes. */

typedef struct _DataPacket_ DataPacket;

/**
 * Template function for reading data packets in and processing them
 * \param data Data buffer where read data should be placed
 * \param size Maximum number of bytes that can be read into data buffer
 * \return The number of bytes actually read
 */
typedef size_t (*DataPacket_ReadFunction)(void* data, const size_t size);

/**
 * Template function for sending data packets out
 * \param data Data buffer containing the data to be sent
 * \param size Number of bytes to send
 * \return The number of bytes transferred
 */
typedef size_t (*DataPacket_WriteFunction)(const void* data, const size_t size);

/**
 * Template function for handling a message received
 * \param dp The data packet on which the message was received
 * \param data The message data
 * \param size Number of bytes in the message
 */
typedef void (*DataPacket_MessageFunction)(const DataPacket* dp, const void* data, const size_t size);

/**
 * Data packet message structure
 */
typedef struct _DataPacketMessage_
{
	uint16_t                   ID;       /**< Message ID */
	DataPacket_MessageFunction Callback; /**< Callback function to handle the received message */
} DataPacketMessage;

/**
 * Data packet structure
 */
typedef struct _DataPacket_
{
	DataPacket_ReadFunction     Read;                        /**< Interface for reading raw data from which to extract packets */
	DataPacket_WriteFunction    Write;                       /**< Interface for writing packets */
	const DataPacketMessage*    Messages;                    /**< NULL terminated array of supported messages */
	uint8_t                     Buffer[DATAPACKET_MAX_SIZE]; /**< Buffer for holding and processing partial packets */
	size_t                      Size;                        /** < Number of bytes currently in the buffer */
} DataPacket;

/**
 * Initialize a data packet instance
 * \param dp The data packet instance to initialize
 * \param messages A NULL terminated array of supported messages
 * \param read_callback Interface function for reading raw data from which to extract packets
 * \param write_callback Interface function for writing packets
 */
void DataPacket_Init(
		DataPacket*                    dp,
		const DataPacketMessage*       messages,
		const DataPacket_ReadFunction  read_interface,
		const DataPacket_WriteFunction write_interface);

/**
 * Process incoming data and extract messages from it
 * \param dp The data packet instance to process incoming data for
 */
void DataPacket_Receive(DataPacket* dp);

/**
 * Send a message using the data packet instance
 * \param dp The data packet instance to send a message from
 * \param messageID ID of the message to send
 * \param data Data buffer which contains the data to be sent
 * \param size Number of bytes in the data buffer
 */
void DataPacket_Send(const DataPacket* dp, const uint8_t messageID, const void* data, const size_t size);

#endif
