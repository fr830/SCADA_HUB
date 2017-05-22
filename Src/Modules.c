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
	},
	{
		ModuleMask_DigitalInput|0x0001,
		ModuleType_DigitalInput,
		1
	},
	{
		ModuleMask_DigitalInput|0x0002,
		ModuleType_DigitalInput,
		2
	},
	{
		ModuleMask_AnalogInput|0x0000,
		ModuleType_AnalogInput,
		0
	}
};

ModulePollFxnHandler handlers[ModuleType_Count] = {
	pollRelay,
	pollDigitalInput,
	pollAnalogInput,
	pollTemperature,
};

void pollRelay(uint16_t moduleNumber){
	ModuleDesc searchDesc;
	searchDesc.moduleNumber = moduleNumber;
	searchDesc.moduleType = ModuleType_Relay;
	uint16_t moduleIdx = findModule(&searchDesc);
	if(moduleIdx == -1)
		return;

	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Read;
	msg.payloadLength = 5;
	msg.messageType = MessageType_ReqWRep;

	uint16_t startReg = 1;
	uint16_t regCount = 2;

	memcpy(msg.payload+1, &startReg, 2);
	memcpy(msg.payload+3, &regCount, 2);

	bool res = SX1278Drv_SendMessageBlocking(&msg,5000);
	if(res){
		relayData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(relayData[moduleNumber].relayState), msg.payload + 7, 2);
		return;
	}
	else{
		relayData[moduleNumber].status = ModuleStatus_Offline;
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
	msg.payloadLength = 5;
	msg.messageType = MessageType_ReqWRep;

	uint16_t startReg = 1;
	uint16_t regCount = 2;

	memcpy(msg.payload+1, &startReg, 2);
	memcpy(msg.payload+3, &regCount, 2);

	bool res = SX1278Drv_SendMessageBlocking(&msg,5000);
	if(res){
		digitalInputData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(digitalInputData[moduleNumber].inputState), msg.payload + 7, 2);
		return;
	}
	else{
		digitalInputData[moduleNumber].status = ModuleStatus_Offline;
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
	msg.payloadLength = 5;
	msg.messageType = MessageType_ReqWRep;

	uint16_t startReg = 1;
	uint16_t regCount = 2;

	memcpy(msg.payload+1, &startReg, 2);
	memcpy(msg.payload+3, &regCount, 2);

	bool res = SX1278Drv_SendMessageBlocking(&msg,5000);
	if(res){
		analogInputData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(analogInputData[moduleNumber].voltage), msg.payload + 7, 2);
		return;
	}
	else{
		analogInputData[moduleNumber].status = ModuleStatus_Offline;
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
	msg.payloadLength = 5;
	msg.messageType = MessageType_ReqWRep;

	uint16_t startReg = 1;
	uint16_t regCount = 2;

	memcpy(msg.payload+1, &startReg, 2);
	memcpy(msg.payload+3, &regCount, 2);

	bool res = SX1278Drv_SendMessageBlocking(&msg,5000);
	if(res){
		temperatureData[moduleNumber].status = ModuleStatus_Online;
		memcpy(&(temperatureData[moduleNumber].temperature), msg.payload + 7, 2);
		return;
	}
	else{
		temperatureData[moduleNumber].status = ModuleStatus_Offline;
		return;
	}
}


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
// returned value - is reg valid
bool getMBRegValue(uint16_t MBAddress, uint16_t regAddress, uint16_t *val){
	ModuleDesc searchDesc;
	switch(MBAddress){
	case ModBusAddress_Relay:
		searchDesc.moduleType = ModuleType_Relay;
		break;
	case ModBusAddress_DigitalInput:
		searchDesc.moduleType = ModuleType_DigitalInput;
		break;
	default:
		return false;
	}

	searchDesc.moduleNumber = regAddress / RegPerModule;

	if(findModule(&searchDesc) == -1)
		return false;

	//ugly workaround

	switch(MBAddress){
	case ModBusAddress_Relay:
		switch(regAddress % RegPerModule){
		case 1:
			*val = relayData[searchDesc.moduleNumber].status;
			break;
		case 2:
			*val = (uint16_t)relayData[searchDesc.moduleNumber].relayState;
			break;
		}
		break;
	case ModBusAddress_DigitalInput:
		switch(regAddress % RegPerModule){
		case 1:
			*val = (uint16_t)digitalInputData[searchDesc.moduleNumber].status;
			break;
		case 2:
			*val = (uint16_t)digitalInputData[searchDesc.moduleNumber].inputState;
			break;
		}
		break;
	default:
		return false;
	}

	*val = ((*val)>>8) | ((*val)<<8);

	return true;
}

bool setMBRegValue(uint16_t MBAddress, uint16_t regAddress, uint16_t *val){
	ModuleDesc searchDesc;
	switch(MBAddress){
	case ModBusAddress_Relay:
		searchDesc.moduleType = ModuleType_Relay;
		break;
	case ModBusAddress_DigitalInput:
		searchDesc.moduleType = ModuleType_DigitalInput;
		break;
	default:
		return false;
	}

	searchDesc.moduleNumber = regAddress / RegPerModule;

	uint16_t moduleIdx = findModule(&searchDesc);

	if(moduleIdx == -1)
		return false;

	LoRa_Message msg;
	msg.address = modules[moduleIdx].LoRaAddress;
	msg.payload[0] = SCADACommandType_Write;
	msg.payloadLength = 5 + 2;
	msg.messageType = MessageType_ReqWRep;

	uint16_t regCount = 1;

	*val = ((*val)>>8) | ((*val)<<8);

	memcpy(msg.payload+1, &regAddress, 2);
	memcpy(msg.payload+3, &regCount, 2);
	memcpy(msg.payload+5, (uint8_t *)val, 2);

	bool res = SX1278Drv_SendMessageBlocking(&msg,10000);

	return res;
}
