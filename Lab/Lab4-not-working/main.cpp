/**
 ============================================================================
 * This program displays an electronic level, it has 2 axis to find the level point.
 * The corsshair moves when the board is rotated.
 * Requires the LCD Elecfreaks LCD interface option when creating a new project
 * It is necessary enable these in Configure.usbdmProject under the "Peripheral Parameters"->FTM tab
 * Select irqHandlingMethod option (Class Method - Software ...)
 ============================================================================
 */

#include "lcd.h" //include lcd functions and types
#include "spi.h" //include serial peripheral interface functions and types
#include "delay.h" //waitMS()
#include "system.h"
#include "derivative.h"
#include "hardware.h"
#include "i2c.h" //i squared c interface
#include "mma845x.h" //accelerometer functions and types
#include <math.h> //sqrt()

#include "Level.h"
#include "Tone.h"

 // Access to USBDM name-space
using namespace USBDM;

/* **************************************************
 *  Globally shared objects representing hardware
 * ************************************************ */
 //I2c interface
I2c0 i2c0;

//accelerometer via I2C
MMA845x accelerometer(i2c0, MMA845x::AccelerometerMode_2Gmode);

// SPI interface
Spi0 spi;

// LCD interface using SPI
Lcd lcd(spi);
/* ************************************************** */

// LCD derived dimensions
int LCD_WIDTH = (LCD_X_MAX - LCD_X_MIN);
int LCD_HEIGHT = (LCD_Y_MAX - LCD_Y_MIN);
int CENTRE_X = ((LCD_X_MAX - LCD_X_MIN) / 2);
int CENTRE_Y = ((LCD_Y_MAX - LCD_Y_MIN) / 2);

// Colour for LCD background
int BACKGROUND_COLOUR = (RED);

// Colour for LCD foreground
int FOREGROUND_COLOUR = (WHITE);

// Radius used for the moving circle
int CIRCLE_RADIUS = (20);

 // Timer being used
using Timer = Ftm1;

// Timer channel for output - change as required
using TimerChannel = Ftm1Channel<1>;

// Half-period for timer in ticks
// This variable is shared with the interrupt routine
uint16_t timerHalfPeriod;

// Waveform period to generate
float WAVEFORM_PERIOD = 5 * ms;

/*
 * Print the accelerometer values to the console.
 */
void report(MMA845x &accelerometer, int &accelStatus, int16_t &accelX, int16_t &accelY, int16_t &accelZ) {
	console.
		write("s=").write(accelStatus, Radix_16).
		write(", aX=").write(accelX).
		write(", aY=").write(accelY).
		write(", aZ=").writeln(accelZ).
		write(", WAVEFORM_PERIOD=").writeln(WAVEFORM_PERIOD);
}

/**
 * Interrupt handler for Timer interrupts
 * This sets the next interrupt/pin toggle for a half-period from the last event
 *
 * @param[in] status Flags indicating interrupt source channel(s)
 */
void ftmCallback(uint8_t status) {

	// Check channel
	if (status & TimerChannel::CHANNEL_MASK) {
		// Note: The pin is toggled directly by hardware
		// Re-trigger at last interrupt time + timerHalfPeriod
		TimerChannel::setDeltaEventTime(timerHalfPeriod);
	}
}

void calibrateAcceleromter(){
	//Calibrate accelerometer
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
}



int main() {
	Level::levelSetup();
	Tone::toneSetup();

	//main program loop
	for (;;) {
		Level::levelLoop();
		Tone::toneLoop();
	}
	return 0;
}

