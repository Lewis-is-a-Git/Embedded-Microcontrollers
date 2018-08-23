/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 23/08/18
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;
//Declare pins used
using inputPins = GpioCField<7, 5>;
using outputPins = GpioAField<7, 0>;

void leds ( unsigned num){
	//map is a lookup table of the pretty pattern
	static const unsigned map[5] = {
	//static to make sure the table is not deleted and recreated each time
	//const make sure the table doesn't change
			0b00000000,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111
	};
	//input validation to not run past the array
	if (num < 5){
		outputPins::write(map[num]); //write the values to the pins
	}
}

int main() {
	//configure pins
	inputPins::setInput();
	outputPins::setOutput();
	//main loop
	while(1) {
		//set the output leds based on the input values
		leds(inputPins::read());
	}
	return 0;
}
