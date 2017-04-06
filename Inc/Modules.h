/*
 * Devices.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Tarolrr
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#define ModuleMask_Relay 0x0000
#define ModuleMask_DigitalInput 0x2000
#define ModuleMask_AnalogInput 0x4000
#define ModuleMask_Temperature 0x6000

#define ModuleCount 2

enum{
	ModuleType_Relay,
	ModuleType_DigitalInput,
	ModuleType_AnalogInput,
	ModuleType_Temperature,
	ModuleType_Count
} typedef ModuleType;

struct{
	uint16_t LoRaAddress;
	ModuleType moduleType;
	uint16_t moduleNumber;
} typedef ModuleDesc;

ModuleDesc modules[ModuleCount];

void PollModules();



#endif /* DEVICES_H_ */
