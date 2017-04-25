/*
 * Devices.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Tarolrr
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include <stdint.h>
#include "SX1278Drv.h"

#define ModuleMask_Relay 0x0000
#define ModuleMask_DigitalInput 0x2000
#define ModuleMask_AnalogInput 0x4000
#define ModuleMask_Temperature 0x6000

#define ModuleCount 2
#define MaxModuleNumber 300

enum{
	SCADACommandType_Read,
	SCADACommandType_Write,
} typedef SCADACommandType;

enum{
	ModuleType_Relay,
	ModuleType_DigitalInput,
	ModuleType_AnalogInput,
	ModuleType_Temperature,
	ModuleType_Count
} typedef ModuleType;

enum{
	ModuleStatus_Offline,
	ModuleStatus_Online,
} typedef ModuleStatus;

struct{
	uint16_t LoRaAddress;
	ModuleType moduleType;
	uint16_t moduleNumber;
	ModuleStatus status;
} typedef ModuleDesc;

ModuleDesc modules[ModuleCount];

typedef struct __attribute__((__packed__)){
	uint8_t status;
	uint16_t relayState; // bit mask
} RelayData;

typedef struct __attribute__((__packed__)){
	uint8_t status;
	uint16_t inputState; // bit mask
} DigitalInputData;

typedef struct __attribute__((__packed__)){
	uint8_t status;
	uint16_t voltage; //in mV units
} AnalogInputData;

typedef struct __attribute__((__packed__)){
	uint8_t status;
	uint16_t temperature; //in 0.1 C degree units
} TemperatureData;

RelayData relayData[MaxModuleNumber];
DigitalInputData digitalInputData[MaxModuleNumber];
AnalogInputData analogInputData[MaxModuleNumber];
TemperatureData temperatureData[MaxModuleNumber];

void PollModules();


#endif /* DEVICES_H_ */
