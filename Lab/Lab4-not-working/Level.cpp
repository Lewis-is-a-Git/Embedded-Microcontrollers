#include "level.h"
using namespace USBDM;
/*
 * Draw the target circle and crossahair, and the central axes
 * @param x: x coordinate of the cursor
 * @param y: y coordinate of the cursor
 * @param colour: colour of the cursor
 */
void Level::drawCursor(int x, int y, int colour) {
	//target circle
	lcd.drawCircle(x, y, CIRCLE_RADIUS, colour);
	lcd.drawLine(x - CIRCLE_RADIUS, y, x + CIRCLE_RADIUS, y, FOREGROUND_COLOUR);
	lcd.drawLine(x, y - CIRCLE_RADIUS, x, y + CIRCLE_RADIUS, FOREGROUND_COLOUR);
	//central axes
	lcd.drawLine(0, LCD_HEIGHT / 2, LCD_WIDTH, LCD_HEIGHT / 2, FOREGROUND_COLOUR);
	lcd.drawLine(LCD_WIDTH / 2, 0, LCD_WIDTH / 2, LCD_HEIGHT, FOREGROUND_COLOUR);
}

void Level::levelSetup() {
	int accelStatus; //accelerator active or inactive.
	int16_t accelX, accelY, accelZ; //variables for the values of the axis roation
	// Check if any USBDM error yet (constructors)
	checkError();

	// Clear the screen
	lcd.clear(BACKGROUND_COLOUR);

	// Setup LCD defaults
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);

	calibrateAcceleromter();

	// Cursor position on screen
	int x = 0, y = 0;
	int xOld = 50, yOld = 50;

	// Set LCD defaults
	lcd.setFont(largeFont).setForeground(BLUE).setBackground(WHITE);
}
void Level::levelLoop() {
	//read acclerometer values
	accelerometer.active();
	waitMS(100);
	accelerometer.readAccelerometerXYZ(accelStatus, accelX, accelY, accelZ);
	accelerometer.standby();

	//display the accelerometer values to the console
	report(accelerometer, accelStatus, accelX, accelY, accelZ);

	//set the cursor coordinates based on the acceleromter values
	//The values are mapped to the screen, to calculate the coordinates
	//and constrained so that the cursor cannot move off the screen
	y = LCD_X_MIN + CIRCLE_RADIUS + ((+accelX + 4096) * (LCD_WIDTH - 2 * CIRCLE_RADIUS) / 8192);
	x = LCD_Y_MIN + CIRCLE_RADIUS + ((-accelY + 4096) * (LCD_HEIGHT - 2 * CIRCLE_RADIUS) / 8192);

	//remove the old cursor, by writing over it withe the background colour
	level::drawCursor(xOld, yOld, BACKGROUND_COLOUR);
	//draw the new cursor
	level::drawCursor(x, y, FOREGROUND_COLOUR);
	//draw the axes
	lcd.drawLine(xOld - CIRCLE_RADIUS, yOld, xOld + CIRCLE_RADIUS, yOld, BACKGROUND_COLOUR);
	lcd.drawLine(xOld, yOld - CIRCLE_RADIUS, xOld, yOld + CIRCLE_RADIUS, BACKGROUND_COLOUR);
	//store the old values
	xOld = x;
	yOld = y;
}
