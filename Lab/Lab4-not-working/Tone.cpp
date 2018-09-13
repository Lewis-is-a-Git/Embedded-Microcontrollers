#include "tone.h"

using namespace USBDM;

void Tone::toneSetup() {
	int accelStatus;
	int16_t accelX, accelY, accelZ;
	checkError();

	calibrateAcceleromter();

	waitMS(100);

	 // Configure base FTM (affects all channels)
	Timer::configure(
		FtmMode_LeftAlign,      // Left-aligned is required for OC/IC
		FtmClockSource_System,  // Bus clock usually
		FtmPrescale_1);         // The prescaler will be re-calculated later

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
		FtmChMode_OutputCompareToggle, //  Output Compare with pin toggle
		FtmChannelAction_Irq);         //  + interrupts on events

// Check if configuration failed
	USBDM::checkError();
}

void Tone::toneLoop() {
	//read accelerometer values
	accelerometer.active();
		waitMS(50);
		accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
		accelerometer.standby();
		report(accelerometer, accelStatus, accelX, accelY, accelZ);

	WAVEFORM_PERIOD = (0.008 - sqrt((accelX * accelX) + (accelY * accelY)) * 0.0025 * ms);

	// Set IC/OC measurement period to longest interval + 10%
	// This adjusts the prescaler value but does not change the clock source
	Timer::setMeasurementPeriod(1.1*WAVEFORM_PERIOD / 2.0);

	// Calculate half-period in timer ticks
	// Must be done after timer clock configuration (above)
	timerHalfPeriod = Timer::convertSecondsToTicks(WAVEFORM_PERIOD / 2.0);

	__asm__("wfi");
}
