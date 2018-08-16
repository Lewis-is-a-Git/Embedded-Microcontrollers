/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 16/8/2018
 *      Author: Brockman-Horsley
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

// LED connection - change as required
using Led   = GpioC<1,ActiveHigh>;
using Button = GpioD<5, ActiveLow>;

void initialise(){
	Led::setOutput(PinDriveStrength_Low, PinDriveMode_PushPull,PinSlewRate_Slow);
	Button::setInput(PinPull_Up, PinAction_None, PinFilter_None);
}

int main() {

	initialise();
	int reactionTime = 0;

	while(true){
		Led::off();
		waitMS(1000); //wait
		Led::on();
		do {
			reactionTime++;
			waitMS(1);
		}while (!Button::isPressed());
		waitMS(10); // wait for bounce

		console.write("Your reaction time is: ").write(reactionTime).write("ms\n");
		reactionTime = 0;
		while(Button::isPressed()){
			//wait for button release
			waitMS(1);
		}
	}
	return 0;
}
