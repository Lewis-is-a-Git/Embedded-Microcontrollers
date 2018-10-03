/**
 * main.cpp
 */
#include "lcd.h"
#include "spi.h"
#include "delay.h"
#include "system.h"
#include "derivative.h"
#include "hardware.h"
#include "i2c.h"
#include "mma845x.h"
#include "level.h"
#include "tone.h"

// Access to USBDM name-space
using namespace USBDM;

/* **************************************************
 *  Globally shared objects representing hardware
 * ************************************************ */
//I2c interface
I2c0 i2c0;

//accelerometer via I2C
MMA845x accelerometer(i2c0, MMA845x::AccelerometerMode_2Gmode);

//Simulate ground pin to connect LCD sreen and speaker at the same time
using ground = GpioC<10>;

/**
 * Initialise the accelerometer, calibrate and check for errors
 * Pass by reference accelerometer data
 */
void initialiseAccelerometer(MMA845x &accelerometer,int &accelStatus,
int16_t &accelX, int16_t &accelY, int16_t &accelZ){
	//Initailse accelerometer
	accelerometer.active();
	waitMS(1000);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();

	//calibrate accelerometer
	waitMS(1000);
	if (accelerometer.calibrateAccelerometer() != E_NO_ERROR) {
		console.write("Calibration failed!\n");
		__asm__("bkpt");
	}
	//check for errors
	checkError();


	//write accelerometer values to console
	console.
		write("s=").write(accelStatus, Radix_16).
		write(", aX=").write(accelX).
		write(", aY=").write(accelY).
		write(", aZ=").writeln(accelZ);
}
/**
 * Read Accelerometer data
 * Pass by reference acceleromter data
 */
void readAccelerometer(MMA845x &accelerometer,int &accelStatus, int16_t &accelX,
int16_t &accelY, int16_t &accelZ) {
	//Read values
	accelerometer.active();
	waitMS(50);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();

	//Write to console
	console.
	write("s=").write(accelStatus, Radix_16).
	write(", aX=").write(accelX).
	write(", aY=").write(accelY).
	write(", aZ=").writeln(accelZ);
}

int main() {
	//Simulate ground pin
	ground::setOutput();
	ground::off();

	//Accelerometer variables to store the accelerometer data
	int accelStatus;
	int16_t accelX,accelY,accelZ;

	//Initialise the accelerometer
	initialiseAccelerometer(accelerometer, accelStatus, accelX, accelY, accelZ);

	//Initialise the interfaces using the accelerometer
	InitialiseLevel(); //LCD Screen to display Level target
	initialiseNotes(); //FTM speaker to play a tone

	//Main loop
	for(;;) {
		//update acceleromter data
		readAccelerometer(accelerometer, accelStatus, accelX, accelY, accelZ);

		//Draw the target and cursor on screen
		drawCursorOnScreen(accelX, accelY);
		//Calculate the frequency based on the accelerometer
		//and set it to the FTM frequency
		setNoteFrequency(calculateFrequency(accelX, accelY));
	}
}
