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

/**
 * See more examples in Snippets directory
 */

// LED connection - change as required
using Led   = GpioA<2,ActiveLow>;

int main() {
	//few gotchas
		//the intention is not to use 3 timer channels
		//easier to use the software
		//generate interrupts
		//6 actions, each change is an event

	//the ftm uses a counter
	//the counter counts
	//output compare mode

	//counter automatically
		//runs callback
		//changes a pin

	//callback
		//sets the next event
	//use a counter to count hte events
		//keep track of shish step we are up to in the sequence

	//after, stop the ftm
		//we dont want anymore events

	//use table driven
		//case statemetns is acceptable for exam



	return 0;
}
