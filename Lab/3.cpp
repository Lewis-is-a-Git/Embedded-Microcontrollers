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

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

// LED connection - change as required
using Led1   = GpioD<5>;
using Led2  = GpioB<1>;
using Led3  = GpioB<0>;

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
	switch(ledNum) {
	case 1  :
		Led1::high();
		Led2::setInactive();
		Led3::low();
	case 2  :
		Led1::low();
		Led2::setInactive();
		Led3::high();
	case 3  :
		Led1::high();
		Led2::low();
		Led3::setInactive();
	case 4  :
		Led1::low();
		Led2::high();
		Led3::setInactive();
	case 5  :
		Led1::setInactive();
		Led2::high();
		Led3::low();
	case 6  :
		Led1::setInactive();
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
