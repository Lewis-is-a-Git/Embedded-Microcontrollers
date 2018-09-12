#include <stdlib.h>
#include "spi.h"
#include "delay.h"
#include <math.h>
#include "system.h"
#include "derivative.h"
#include "hardware.h"
#include "i2c.h"
#include "mma845x.h"
#include "delay.h"

using namespace USBDM;

/**
 * This example uses FTM interrupts.
 *
 * It is necessary enable these in Configure.usbdmProject under the "Peripheral Parameters"->FTM tab
 * Select irqHandlingMethod option (Class Method - Software ...)
 */

// Timer being used - change as required
// Could also access as TimerChannel::Ftm
using Timer = Ftm1;

// Timer channel for output - change as required
using TimerChannel = Ftm1Channel<1>;

//actual code
//I2c interface
I2c0 i2c0;

//accelerometer via I2C
MMA845x accelerometer(i2c0, MMA845x::AccelerometerMode_2Gmode);



//static_assert(std::is_same<TimerChannel::Ftm,Timer>::value, "Timer channel must belong to same Timer!");

// Half-period for timer in ticks
// This variable is shared with the interrupt routine
static volatile uint16_t timerHalfPeriod;

// Waveform period to generate
float WAVEFORM_PERIOD = 5*ms;

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

void report(MMA845x &accelerometer,int &accelStatus, int16_t &accelX, int16_t &accelY, int16_t &accelZ) {



	console.
	write("s=").write(accelStatus, Radix_16).
	write(", aX=").write(accelX).
	write(", aY=").write(accelY).
	write(", aZ=").write(accelZ).
	write(", WAVEFORM_PERIOD=").writeln(WAVEFORM_PERIOD);
}

int main() {
	int accelStatus;
	int16_t accelX,accelY,accelZ;
	checkError();

	accelerometer.active();
	waitMS(1000);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();
	report(accelerometer, accelStatus, accelX, accelY, accelZ);
	console.write("doing simple calibration\n"
			"make sure the device is level!\n");

	waitMS(1000);
	if (accelerometer.calibrateAccelerometer() != E_NO_ERROR) {
		console.write("Calibration failed!\n");
		__asm__("bkpt");
	}

	waitMS(100);

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
	//Timer::setMeasurementPeriod(1.1*WAVEFORM_PERIOD/2.0);

	// Calculate half-period in timer ticks
	// Must be done after timer clock configuration (above)
	//timerHalfPeriod = Timer::convertSecondsToTicks(WAVEFORM_PERIOD/2.0);

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

	for(;;) {
		float WAVEFORM = (0.008 - sqrt((accelX * accelX) + (accelY * accelY) ) * 0.0025 * ms);
		WAVEFORM_PERIOD = WAVEFORM;

		Timer::setMeasurementPeriod(1.1*WAVEFORM_PERIOD/2.0);
		timerHalfPeriod = Timer::convertSecondsToTicks(WAVEFORM_PERIOD/2.0);

		accelerometer.active();
		waitMS(50);
		accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
		accelerometer.standby();
		report(accelerometer, accelStatus, accelX, accelY, accelZ);
		__asm__("wfi");
	}
	return 0;
}

