#include <stdlib.h>
#include "lcd.h"
#include "spi.h"
#include "delay.h"
#include <math.h>
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

using ground = GpioC<10>;



void initialiseAccelerometer(MMA845x &accelerometer,int &accelStatus, int16_t &accelX, int16_t &accelY, int16_t &accelZ){
	accelerometer.active();
	waitMS(1000);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();

	waitMS(1000);
	if (accelerometer.calibrateAccelerometer() != E_NO_ERROR) {
		console.write("Calibration failed!\n");
		__asm__("bkpt");
	}
	checkError();

	console.
	write("s=").write(accelStatus, Radix_16).
	write(", aX=").write(accelX).
	write(", aY=").write(accelY).
	write(", aZ=").writeln(accelZ);
}

void ReadAccel(MMA845x &accelerometer,int &accelStatus, int16_t &accelX, int16_t &accelY, int16_t &accelZ) {

	accelerometer.active();
	waitMS(50);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();

	console.
	write("s=").write(accelStatus, Radix_16).
	write(", aX=").write(accelX).
	write(", aY=").write(accelY).
	write(", aZ=").writeln(accelZ);
}



int main() {

	ground::setOutput();
	ground::off();

	int accelStatus;
	int16_t accelX,accelY,accelZ;

	initialiseAccelerometer(accelerometer, accelStatus, accelX, accelY, accelZ);

	InitialiseLevel();
	initialiseNotes();

	for(;;) {

		ReadAccel(accelerometer, accelStatus, accelX, accelY, accelZ);

		drawCursorOnScreen(accelX, accelY);
		int frequency = calculateFrequency(accelX, accelY);
		setNoteFrequency(frequency);
	}
}
