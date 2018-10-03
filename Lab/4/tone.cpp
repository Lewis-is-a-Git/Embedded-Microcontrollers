/**
 * tone.cpp
 */
#include "tone.h"

#include "hardware.h" //FTM
#include <math.h> //sqrt

using namespace USBDM;

/**
 * This example uses FTM interrupts.
 *
 * It is necessary enable these in Configure.usbdmProject under the
 * "Peripheral Parameters"->FTM tab
 * Select irqHandlingMethod option (Class Method - Software ...)
 */

// Timer being used
using Timer = Ftm1;

// Timer channel for output
using TimerChannel = Ftm1Channel<1>;

// Half-period for timer in ticks
// This variable is shared with the interrupt routine
static volatile uint16_t timerHalfPeriod;

// Waveform period to generate
static constexpr float WAVEFORM_PERIOD = 100*ms;

//offset to make the frequency higher as it tilts more
static constexpr float OFFSET = 0.008;

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

/**
 *  Calculate the frequency based on accelerometer data
 *  @param accelX - X axis of accelerometer
 *  @param accelY - Y axis of accelerometer
 */
int calculateFrequency(int16_t accelX, int16_t accelY){
	return sqrt((accelX * accelX) + (accelY * accelY) );
}

void initialiseNotes(void)
{
	/**
	 * FTM channel set as Output compare with pin Toggle mode and using
 	 * a callback function
	 */
	// Configure base FTM (affects all channels)
	Timer::configure(
			FtmMode_LeftAlign,      // Left-aligned is required for OC/IC
			FtmClockSource_System,  // Bus clock usually
			FtmPrescale_1);         // The prescaler will be re-calculated
	// Set IC/OC measurement period to longest interval + 10%
	// This adjusts the prescaler value but does not change the clock source
	Timer::setMeasurementPeriod(1.1*WAVEFORM_PERIOD/2.0);

	// Calculate half-period in timer ticks
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

	// Trigger 1st interrupt at now+100
	TimerChannel::setRelativeEventTime(100);

	// Configure the channel
	TimerChannel::configure(
		//  Output Compare with pin toggle
		FtmChMode_OutputCompareToggle,
FtmChannelAction_Irq);         //  + interrupts on events

	// Check if configuration failed
	USBDM::checkError();
}

/**
 * Changes the frequency of the note being played
 *
 * @param frequency - Frequency of note (100Hz-10kHz)
 *
 * @note This function returns immediately.
 *       The note is played by interrupt driven code.
 */
void setNoteFrequency(unsigned frequency){
	//calculate timer half period in ticks
	if (frequency >= NOTE_MIN && frequency <= NOTE_MAX){
		timerHalfPeriod = Timer::convertSecondsToTicks((1.0/frequency)/2.0);
	}
}

