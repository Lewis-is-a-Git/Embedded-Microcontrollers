/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 19/8/2018
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

// LED connection - change as required
using Led1   = GpioD<1>;
using Led2  = GpioD<3>;
using Led3  = GpioD<2>;

/*
 * Initialise the Charlieplexed system
 */
void initialiseCharlieplexing(){
	Led1::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);
	Led2::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);
	Led3::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);
}
/*
 * Turn on the given LED
 *
 * @param ledNum LED to turn on (1..6)
 *
 */
void setLED(int ledNum){
	Led1::setOut();
	Led2::setOut();
	Led3::setOut();
	switch(ledNum) {
	case 1  :
		Led1::high();
		Led2::setIn();
		Led3::low();
	case 2  :
		Led1::low();
		Led2::setIn();
		Led3::high();
	case 3  :
		Led1::high();
		Led2::low();
		Led3::setIn();
	case 4  :
		Led1::low();
		Led2::high();
		Led3::setIn();
	case 5  :
		Led1::setIn();
		Led2::high();
		Led3::low();
	case 6  :
		Led1::setIn();
		Led2::low();
		Led3::high();
	default : //Optional
		Led1::low();
		Led2::low();
		Led3::low();
	}

}

int main() {

	void initialiseCharlieplexing();

	for(int ledNum = 1;;ledNum++) {
		if (ledNum > 6){
			ledNum = 1;
		}
		setLED(ledNum);
		waitMS(100);
		console.write("The led is ").writeln(ledNum);
	}
	return 0;
}
