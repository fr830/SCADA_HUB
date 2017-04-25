/*
 * Modules.c
 *
 *  Created on: Apr 3, 2017
 *      Author: Tarolrr
 */

#include <Modules.h>

extern volatile bool LoRaError; // TODO ugly workaroung
extern volatile bool LoRaSuccess; // TODO ugly workaroung
extern volatile uint8_t LoRaData[64];

typedef void( *ModulePollFxnHandler ) (uint16_t moduleNumber);

static uint16_t findModule(ModuleDesc *desc);

void pollRelay(uint16_t moduleNumber);
void pollDigitalInput(uint16_t moduleNumber);
void pollAnalogInput(uint16_t moduleNumber);
void pollTemperature(uint16_t moduleNumber);

ModulePollFxnHandler handlers[ModuleType_Count] = {
	pollRelay,
	pollDigitalInput,
	pollAnalogInput,
	pollTemperature,
};

void pollRelay(uint16_t moduleNumber){
	ModuleDesc searchDesc;
	searchDesc.moduleNumber = moduleNumber;
	searchDesc.moduleType = ModuleType_DigitalInput;
	uint16_t moduleIdx = findModule(&searchDesc);
	if(moduleIdx == -1)
		return;
	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Read;
	uint16_t startReg = 1;
	memcpy(msg.payload+1, &startReg, 2);
	uint16_t regCount = 2;
	memcpy(msg.payload+3, &regCount, 2);
	msg.payloadLength = 5;
	LoRaError = false;
	LoRaSuccess = false;
	SX1278Drv_SendMessage(&msg);
	while((!LoRaSuccess)&&(!LoRaError));
	if(LoRaError){
		relayData[moduleNumber].status = ModuleStatus_Offline;
		return;
	}
	if(LoRaSuccess){
		relayData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(relayData[moduleNumber].relayState), LoRaData, 2);
		return;
	}
}

void pollDigitalInput(uint16_t moduleNumber){
	ModuleDesc searchDesc;
	searchDesc.moduleNumber = moduleNumber;
	searchDesc.moduleType = ModuleType_DigitalInput;
	uint16_t moduleIdx = findModule(&searchDesc);
	if(moduleIdx == -1)
		return;
	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Read;
	uint16_t startReg = 1;
	memcpy(msg.payload+1, &startReg, 2);
	uint16_t regCount = 2;
	memcpy(msg.payload+3, &regCount, 2);
	msg.payloadLength = 5;
	LoRaError = false;
	LoRaSuccess = false;
	SX1278Drv_SendMessage(&msg);
	while((!LoRaSuccess)&&(!LoRaError));
	if(LoRaError){
		digitalInputData[moduleNumber].status = ModuleStatus_Offline;
		return;
	}
	if(LoRaSuccess){
		digitalInputData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(digitalInputData[moduleNumber].inputState), LoRaData, 2);
		return;
	}
}

void pollAnalogInput(uint16_t moduleNumber){
	ModuleDesc searchDesc;
	searchDesc.moduleNumber = moduleNumber;
	searchDesc.moduleType = ModuleType_AnalogInput;
	uint16_t moduleIdx = findModule(&searchDesc);
	if(moduleIdx == -1)
		return;
	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Read;
	uint16_t startReg = 1;
	memcpy(msg.payload+1, &startReg, 2);
	uint16_t regCount = 2;
	memcpy(msg.payload+3, &regCount, 2);
	msg.payloadLength = 5;
	LoRaError = false;
	LoRaSuccess = false;
	SX1278Drv_SendMessage(&msg);
	while((!LoRaSuccess)&&(!LoRaError));
	if(LoRaError){
		analogInputData[moduleNumber].status = ModuleStatus_Offline;
		return;
	}
	if(LoRaSuccess){
		analogInputData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(analogInputData[moduleNumber].voltage), LoRaData, 2);
		return;
	}
}

void pollTemperature(uint16_t moduleNumber){
	ModuleDesc searchDesc;
	searchDesc.moduleNumber = moduleNumber;
	searchDesc.moduleType = ModuleType_Temperature;
	uint16_t moduleIdx = findModule(&searchDesc);
	if(moduleIdx == -1)
		return;
	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Read;
	uint16_t startReg = 1;
	memcpy(msg.payload+1, &startReg, 2);
	uint16_t regCount = 2;
	memcpy(msg.payload+3, &regCount, 2);
	msg.payloadLength = 5;
	LoRaError = false;
	LoRaSuccess = false;
	SX1278Drv_SendMessage(&msg);
	while((!LoRaSuccess)&&(!LoRaError));
	if(LoRaError){
		temperatureData[moduleNumber].status = ModuleStatus_Offline;
		return;
	}
	if(LoRaSuccess){
		temperatureData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(temperatureData[moduleNumber].temperature), LoRaData, 2);
		return;
	}
}

ModuleDesc modules[ModuleCount] = {
	{
		ModuleMask_Relay|0x0000,
		ModuleType_Relay,
		0
	},
	{
		ModuleMask_DigitalInput|0x0000,
		ModuleType_DigitalInput,
		0
	}
};

static uint16_t findModule(ModuleDesc *desc){
	uint16_t i;
	for(i = 0; i < ModuleCount; i++){
		if( (modules[i].moduleNumber == desc->moduleNumber) &&
			(modules[i].moduleType == desc->moduleType))
			return i;
	}
	return -1;
}

void PollModules(){
	uint16_t i;
	for(i = 0; i < ModuleCount; i++){
		handlers[modules[i].moduleType](modules[i].moduleNumber);
	}
}
