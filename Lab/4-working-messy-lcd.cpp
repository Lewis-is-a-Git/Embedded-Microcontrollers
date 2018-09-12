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
#include "delay.h"

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

/// LCD derived dimensions
static constexpr int LCD_WIDTH  = (LCD_X_MAX-LCD_X_MIN);
static constexpr int LCD_HEIGHT = (LCD_Y_MAX-LCD_Y_MIN);
static constexpr int CENTRE_X   = ((LCD_X_MAX-LCD_X_MIN)/2);
static constexpr int CENTRE_Y   = ((LCD_Y_MAX-LCD_Y_MIN)/2);

// Colour for LCD background
static constexpr int BACKGROUND_COLOUR = (RED);

// Colour for LCD foreground
static constexpr int FOREGROUND_COLOUR = (WHITE);

// Radius used for the moving circle
static constexpr int CIRCLE_RADIUS = (20);

void report(MMA845x &accelerometer,int &accelStatus, int16_t &accelX, int16_t &accelY, int16_t &accelZ) {



	console.
	write("s=").write(accelStatus, Radix_16).
	write(", aX=").write(accelX).
	write(", aY=").write(accelY).
	write(", aZ=").writeln(accelZ);
}


void drawCursor(int x, int y, int colour) {
	lcd.drawCircle(x, y, CIRCLE_RADIUS, colour);
	lcd.drawLine(0,LCD_HEIGHT/2,LCD_WIDTH,LCD_HEIGHT/2,FOREGROUND_COLOUR);
	lcd.drawLine(LCD_WIDTH/2,0,LCD_WIDTH/2,LCD_HEIGHT,FOREGROUND_COLOUR);

	lcd.drawLine(x-CIRCLE_RADIUS,y,x+CIRCLE_RADIUS,y,FOREGROUND_COLOUR);
	lcd.drawLine(x,y-CIRCLE_RADIUS,x,y+CIRCLE_RADIUS,FOREGROUND_COLOUR);

}

int main() {
	int accelStatus;
	int16_t accelX,accelY,accelZ;
	checkError();

	// Draw Crosshair
	lcd.clear(BACKGROUND_COLOUR);

	// Set LCD defaults
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);

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

	// Cursor position on screen
	int x=0, y=0;
	int xOld=50, yOld=50;

	// Set LCD defaults
	lcd.setFont(largeFont).setForeground(BLUE).setBackground(WHITE);


	for(;;) {
		accelerometer.active();
		waitMS(100);
		accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
		accelerometer.standby();
		report(accelerometer, accelStatus, accelX, accelY, accelZ);
		y = LCD_X_MIN + CIRCLE_RADIUS + ((+accelX + 4095) * (LCD_WIDTH-2*CIRCLE_RADIUS) / 8200);
		x = LCD_Y_MIN + CIRCLE_RADIUS + ((-accelY + 4095) * (LCD_HEIGHT-2*CIRCLE_RADIUS) / 8200);
		drawCursor(xOld, yOld, BACKGROUND_COLOUR);
		drawCursor(x, y, FOREGROUND_COLOUR);
		lcd.drawLine(xOld-CIRCLE_RADIUS,yOld,xOld+CIRCLE_RADIUS,yOld,BACKGROUND_COLOUR);
		lcd.drawLine(xOld,yOld-CIRCLE_RADIUS,xOld,yOld+CIRCLE_RADIUS,BACKGROUND_COLOUR);
		xOld = x;
		yOld = y;
	}
}
