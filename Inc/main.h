#ifndef __MAIN_H
#define __MAIN_H

#include <Modules.h>

const uint64_t GUID[] = {
	0x2B2CB0ED7CAA4FED,
	0xBAD921D6CA8A536B
};

//constant for LoRa driver

const uint8_t NetworkID = 0x01;
const uint16_t LoRaAddress = 0x0000;

/********************
 * SCADA master packet structure:
 * 		offset		|		size		|		description
 * 		0			|		1			|		command type (several standard and several device specific)
 * 		1			|	up to 64		|		command argument
 ********************/

/*********************
 * Read command structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 */

/*********************
 * Read reply structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 * 		4			|	up to 64		|		register data
 */

/*********************
 * Write command structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 * 		4			|	up to 64		|		register data
 */

/*********************
 * Write reply structure:
 * 		offset		|		size		|		description
 * 		0			|		2			|		register address
 * 		2			|		2			|		register count
 */

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
