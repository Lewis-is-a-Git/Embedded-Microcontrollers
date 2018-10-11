/*
 * servo.cpp
 *
 *  Created on: 11 Oct 2018
 *      Author: Lewis
 */
#include "servo.h"
#include "hardware.h"

using namespace USBDM;

// Connection mapping
using Timer = Ftm0;
using Servo   = Ftm0Channel<6>;

constexpr float SERVO_PERIOD = 20.0*ms;

/**
 * Initialises the servo motor.
 * Initial position is middle (50%).
 */
void initialiseServo(){

	// Configure base FTM for left-aligned PWM
	Timer::configure(
			FtmMode_LeftAlign,
			FtmClockSource_System,
			FtmPrescale_1
	);

	/*
	 * Change PWM period
	 * Servo period is 20ms
	 */
	Timer::setPeriod(SERVO_PERIOD);

	// Configure channel as PWM high-pulses
	Servo::configure(FtmChMode_PwmHighTruePulses);

	// Configure pin associated with channel
	Servo::setDriveStrength(PinDriveStrength_High);
	Servo::setDriveMode(PinDriveMode_PushPull);

	//set the servo for output
	Servo::setOutput();

	// Check if configuration faiServo
	checkError();
}
/**
 * Set servo position.
 *
 * @param position Position to set (0-100)
 *
 */
void setServoPosition(unsigned position){
	//map integers 0 - 100 to 1ms - 2ms
	Servo::setHighTime((1*ms/100)*position+(1*ms));
}


