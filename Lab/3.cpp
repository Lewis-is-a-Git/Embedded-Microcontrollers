/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 20/8/18
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

// LED connection - change as required
using Leds = GpioDField<3, 1>;

using Adc = Adc0;
using Voltimeter = Adc0Channel<7>;

/*
 * Initialise the Charlieplexed system
 */
void initialiseCharlieplexing(){
	Leds::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);
}
/*
 * Turn on the given LED
 *
 * @param ledNum LED to turn on (1..6)
 *
 */
void setLED(int led){
	//TODO: write(0b110) //use table driven program not this case statement
	//TODO: setDirection(0b101) or whatever values needed, which one is out and in?
	//TODO: look at gpio field functions
	//Leds::setDirection(0b111);
	//Leds::write(0b000);
	switch(led) {
	case 1  :
		Leds::setDirection(0b101);
		Leds::write(0b100);
	case 2  :
		Leds::setDirection(0b101);
		Leds::write(0b001);
	case 3  :
		Leds::setDirection(0b110);
		Leds::write(0b100);
	case 4  :
		Leds::setDirection(0b110);
		Leds::write(0b010);
	case 5  :
		Leds::setDirection(0b011);
		Leds::write(0b010);
	case 6  :
		Leds::setDirection(0b011);
		Leds::write(0b001);
	}

}

/*
 * Initialisation of analogue to digital converter
 */
void initADC(){
	Adc::configure(AdcResolution_12bit_se, AdcClockSource_Bus, AdcClockDivider_2);
	Adc::calibrate();
	Adc::setAveraging(AdcAveraging_4);
}

int main() {

	initialiseCharlieplexing();

	for(int ledNum = 1;;ledNum++) {
		//Part 1
		if (ledNum > 6){
			ledNum = 1;
		}
		//TODO: part 1 needs to be redone
		setLED(ledNum);
		waitMS(100);
		console.write("The led is ").writeln(ledNum);

		//Part 2
		initADC();
		double Level = Voltimeter::readAnalogue();
		double Voltage = Level * 33/40960;
		console.write("ADC Channel 7b = ").write(Voltage).writeln("V.");
	}
	return 0;
}
