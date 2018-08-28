/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   charlieplexing and ADC
 ============================================================================
 */
#include "hardware.h"
#include <cmath> //round

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
	Leds::setInOut(PinPull_Up, PinDriveStrength_High);
}
/*
 * Turn on the given LED
 *
 * @param led LED to turn on (1..6)
 *
 */
void setLed(unsigned led){
	static const unsigned direction[6] = {
			0b101,
			0b101,
			0b110,
			0b110,
			0b011,
			0b011
	};
	static const unsigned write[6] = {
			0b100,
			0b001,
			0b100,
			0b010,
			0b010,
			0b001
	};

	if (led <= 5){
		Leds::write(write[led]);
		Leds::setDirection(direction[led]);

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

	for (;;)
	{
		//Part 1: cycle LEDs
//		for (int i = 0; i < 6; i++)
//		{
//			setLed(i);
//			waitMS(100);
//		}

		//Part 2: ADC
		initADC();
		double Level = Voltimeter::readAnalogue();
		double Voltage = Level * 33 / 40960;
		console.write("ADC Channel 7b = ").write(Voltage).writeln("V.");

		//Part 3: Integration
		int ledDisplay = round(Level * 5 / 4096);
		setLed(ledDisplay);
		console.write("The Led to display is ").writeln(ledDisplay);
		waitMS(100);
	}
	return 0;
}
