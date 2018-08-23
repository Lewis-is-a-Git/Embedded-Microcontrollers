/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 *
 *  Created on: 23/8/18
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

//declare the pins used
using Led  = GpioC<1>; //heater
using Adc  = Adc0; //adc channel 0
using Pot = Adc0Channel<14>; //potentiometer to emulate a thermocouple
/**
 * Initialise the adc
 */
void adcInit(){
	Adc::configure(
			AdcResolution_12bit_se,
			AdcClockSource_Bus,
			AdcClockDivider_2);
	Adc::calibrate();
	Adc::setAveraging(AdcAveraging_4);
}

int main() {
	//set initial variables
	adcInit();
	double SetTemp = 20; //initial temperature is set to 20
	Led::setOutput();

	while(1){

		double Level = Pot::readAnalogue(); //read pot value
		double Temperature = (Level * 20 / 4095) + 15; //converto to degrees
		console.write("The Temperature is: ").writeln(Temperature);
		console.write("The Temperature is set to: ").writeln(SetTemp);
		waitMS(1000);
		//emulate histersis of +2 and -2 degrees
		if (SetTemp < Temperature - 2){
			Led::low();
		}
		else if (SetTemp > Temperature + 2){
			Led::high();
		};
	}
	return 0;
}
