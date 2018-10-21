/*
 ============================================================================
 * @brief   Data Logger
 ============================================================================
 */
#include "hardware.h"
#include "display.h"
#include "capture.h"

using namespace USBDM;

int main() {
	//Initialise joystick
	joystickInit();

	//Initialise the display
	displayInit();
	//Display the menu
	drawMenu(0);

	//Vaiable to make sure the data is only displayed once
	bool finished = false;

	//Main for loop
	for(;;){
		//Poll joystick inputs
		readJoyStick();

		//Check if capture is complete and if it has plotted data already
		if (isCaptureComplete() && !finished){
			displayData();
			finished = true;
		}
	}
}
