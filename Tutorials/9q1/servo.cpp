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

constexpr float SERVO_PERIOD    = 20*ms;
constexpr float SERVO_MIN_PULSE = 1*ms;
constexpr float SERVO_MAX_PULSE = 2*ms;

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
	Servo::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

	// Check if configuration faiServo
	checkError();

	//initial position is 50%
	setServoPosition(50);

}
/**
 * Set servo position.
 *
 * @param position Position to set (0-100)
 *
 */
void setServoPosition(unsigned position){
	//Defensive programming
	if (position > 100){
		position = 100;
	}

	//map integers 0 - 100 to 1ms - 2ms
	Servo::setHighTime(((SERVO_MAX_PULSE - SERVO_MIN_PULSE)/100.0) * position + (SERVO_MIN_PULSE));
}


