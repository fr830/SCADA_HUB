#ifndef __MAIN_H
#define __MAIN_H

const uint64_t GUID[] = {
	0x2B2CB0ED7CAA4FED,
	0xBAD921D6CA8A536B
};

//constant for LoRa driver

const uint8_t NetworkID = 0x01;
const uint16_t LoRaAddress = 0x0000;


enum{
	LoRaPacketType_MODBUSBPacket,
	LoRaPacketType_SCADAPacket
} typedef LoRaPacketType;

enum{
	SCADADeviceType_Hub,
	SCADADeviceType_Relay,
	SCADADeviceType_Analog,
} typedef SCADADeviceType;

enum{
	SCADACommandType_Read,
	SCADACommandType_Write,
} typedef SCADACommandType;
/********************
 * SCADA master packet structure:
 * 		offset		|		size		|		description
 * 		0			|		1			|		device type
 * 		1			|		1			|		command type (several standard and several device specific)
 * 		2			|	up to 64		|		command argument
 ********************/

/*********************
 * Read command structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 */

/*********************
 * Write command structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 * 		4			|	up to 64		|		register data
 */

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
