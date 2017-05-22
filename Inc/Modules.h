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

#define ModuleMask_Relay 0x1000
#define ModuleMask_DigitalInput 0x2000
#define ModuleMask_AnalogInput 0x3000
#define ModuleMask_Temperature 0x4000

#define ModuleCount 5
#define MaxModuleNumber 300
#define RegPerModule 32
enum{
	ModBusAddress_Relay = 30,
	ModBusAddress_DigitalInput = 31,
} typedef ModBusAddress;

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
bool getMBRegValue(uint16_t MBAddress, uint16_t regAddress, uint16_t *val);
bool setMBRegValue(uint16_t MBAddress, uint16_t regAddress, uint16_t *val);

#endif /* DEVICES_H_ */
