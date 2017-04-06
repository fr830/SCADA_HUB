/*
 * Modules.c
 *
 *  Created on: Apr 3, 2017
 *      Author: Tarolrr
 */

#include <Modules.h>

typedef void( *ModulePollFxnHandler ) ();

ModulePollFxnHandler handlers[ModuleType_Count];

void pollRelay(){

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

void PollModules(){
	uint16_t i;
	for(i = 0; i < ModuleCount; i++){

	}
}
