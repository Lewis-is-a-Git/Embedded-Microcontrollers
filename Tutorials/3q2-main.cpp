/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   house alarm
 *
 *  Created on: 23/8/2018
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

using Laser = GpioC<0>;
using PressureMat = GpioC<3, ActiveLow>;
using Door = GpioC<4, ActiveLow>;
using Key = GpioC<5>;
/**
 * Initalise pin directions
 * pressure mat and door require pull up resistors
 */
void initPins(){
	Laser::setOutput(PinDriveStrength_High, 
			PinDriveMode_PushPull, 
			PinSlewRate_Slow);
	PressureMat::setInput(PinPull_Up, PinAction_None, PinFilter_None);
	Door::setInput(PinPull_Up, PinAction_None, PinFilter_None);
	Key::setInput(PinPull_Up, PinAction_None, PinFilter_None);
}
int main() {
	initPins();

	while (true){
		//if the system is enabled
		if (Key::isActive() == 1){ 
			//if one of the triggers are activated
			if (PressureMat::isActive() == 0 | Door::isActive() == 0){
				Laser::high(); //laser output high
			}
		}
		else {
			Laser::low();
		}
	}
	return 0;
}
