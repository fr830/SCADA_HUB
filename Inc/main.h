#ifndef __MAIN_H
#define __MAIN_H

#include <Modules.h>

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
