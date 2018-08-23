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
#include <cmath>

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
 * @param led LED to turn on (1..6)
 *
 */
void setLED(unsigned led){
	static const unsigned direction[6] = {
			0b101,
			0b101,
			0b110,
			0b110,
			0b011,
			0b011};
	static const unsigned write[6] = {
			0b100,
			0b001,
			0b100,
			0b010,
			0b010,
			0b001};
	if (led == 0){
		Leds::setDirection(0b111);
		Leds::write(0b000);
	}
	if (led < 7){
		Leds::setDirection(direction[led - 1]);
		Leds::write(write[led - 1]);
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
		//cycle Leds
		//setLED(ledNum);
//		waitMS(100);
//		console.write("The led is ").writeln(ledNum);

		//Part 2
		initADC();
		double Level = Voltimeter::readAnalogue();
		double Voltage = Level * 33/40960;
		console.write("ADC Channel 7b = ").write(Voltage).writeln("V.");

		//Part 3
		int ledDisplay = round(Level * 60/40960);
		setLED(ledDisplay);
		console.write("The Led to display is ").writeln(ledDisplay);

	}
	return 0;
}
