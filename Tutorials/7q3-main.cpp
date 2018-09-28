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
using PushButton   = GpioC<0>;
using InputDial = GpioCField<7, 1>;

using Timer = Ftm0;
using TimerChannel = Ftm0Channel<5>;

// Half-period for timer in ticks
// This variable is shared with the interrupt routine
static volatile uint16_t timerHalfPeriod;

// Waveform period to generate
static constexpr float WAVEFORM_PERIOD = 100*ms;

/**
 * Interrupt handler for Timer interrupts
 * This sets the next interrupt/pin toggle for a half-period from the last event
 *
 * @param[in] status Flags indicating interrupt source channel(s)
 */
static void ftmCallback(uint8_t status) {

	// Check channel
	if (status & TimerChannel::CHANNEL_MASK) {
		// Note: The pin is toggled directly by hardware
		// Re-trigger at last interrupt time + timerHalfPeriod
		TimerChannel::setDeltaEventTime(timerHalfPeriod);
	}

}

int main() {

	//set up the ftm

	/**
	 * FTM channel set as Output compare with pin Toggle mode and using a callback function
	 */
	// Configure base FTM (affects all channels)
	Timer::configure(
			FtmMode_LeftAlign,      // Left-aligned is required for OC/IC
			FtmClockSource_System,  // Bus clock usually
			FtmPrescale_1);         // The prescaler will be re-calculated later

	// Set IC/OC measurement period to longest interval + 10%
	// This adjusts the prescaler value but does not change the clock source
	Timer::setMeasurementPeriod(1.1*WAVEFORM_PERIOD/2.0);

	// Calculate half-period in timer ticks
	// Must be done after timer clock configuration (above)
	timerHalfPeriod = Timer::convertSecondsToTicks(WAVEFORM_PERIOD/2.0);

	// Set callback function
	Timer::setChannelCallback(ftmCallback);

	// Enable interrupts for entire timer
	Timer::enableNvicInterrupts();

	// Configure pin associated with channel
	TimerChannel::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
	// or change individual attributes
	//  TimerChannel::setDriveStrength(PinDriveStrength_High);
	//  TimerChannel::setDriveMode(PinDriveMode_PushPull);

	// Trigger 1st interrupt at now+100
	TimerChannel::setRelativeEventTime(100);

	// Configure the channel
	TimerChannel::configure(
			FtmChMode_OutputCompareToggle, //  Output Compare with pin toggle
			FtmChannelAction_Irq);         //  + interrupts on events

	// Check if configuration failed
	USBDM::checkError();

	//set the pin high
	//set the pin low
	//stop the ftm

	//interval is a variable
	//controlled by dial

	//the system work in ticks
	//need to convert the time to ticks

	//clock frequency and prescaller affect ticks
	//convertSecondsToTicks

		//output compet with pin set
			//pin clear on next event

	PushButton::setInput();
	InputDial::setInput();

	int buttonPressed, oldButtonPressed;

	for(;;) {
		buttonPressed = PushButton::read();
		if (buttonPressed && !oldButtonPressed){
			//sometign is done in the handler
		}
		oldButtonPressed = buttonPressed;
	}
	return 0;
}
