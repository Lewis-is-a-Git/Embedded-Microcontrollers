/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 10/1/2016
 *      Author: podonoghue
 ============================================================================
 */
#include "hardware.h"
#include "display.h"
#include "capture.h"

using namespace USBDM;


int main() {
	joystickInit();
	drawMenu(0);
	bool finished = false;

	for(;;){
		readJoyStick();
		if (isCaptureComplete() && !finished){
			displayData();
			finished = true;
		}
	}
}
