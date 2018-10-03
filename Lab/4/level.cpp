/*
 * level.cpp
 */
#include "level.h"

#include "lcd.h"
#include "delay.h"

using namespace USBDM;

// SPI interface
Spi0 spi;

// LCD interface using SPI
Lcd lcd(spi);

/// LCD derived dimensions
constexpr int LCD_WIDTH  = (LCD_X_MAX-LCD_X_MIN);
constexpr int LCD_HEIGHT = (LCD_Y_MAX-LCD_Y_MIN);
static constexpr int CENTRE_X   = ((LCD_X_MAX-LCD_X_MIN)/2);
static constexpr int CENTRE_Y   = ((LCD_Y_MAX-LCD_Y_MIN)/2);

// Colour for LCD background
static constexpr int BACKGROUND_COLOUR = (RED);

// Colour for LCD foreground
static constexpr int FOREGROUND_COLOUR = (WHITE);

// Radius used for the moving circle
constexpr int CIRCLE_RADIUS = (20);

//Resolution of accelerometer
constexpr int ACCELEROMETER_RANGE = 4096;

//Coordiantes on LCD screen
int x=0, y=0;
int xOld=50, yOld=50; //old Coordinates to clear the target

/*
 * Initailise the LCD level Display
 */
void InitialiseLevel(){
	//Clears the screen
	lcd.clear(BACKGROUND_COLOUR);
	// Set LCD defaults
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR)
.setBackground(BACKGROUND_COLOUR);
	waitMS(100);
	// Set LCD defaults
	lcd.setFont(largeFont).setForeground(BLUE).setBackground(WHITE);
}

/*
 * Draws the target circle to the LCD
 */
void drawCursor(int x, int y, int colour) {
	//Draw Target Circle
	lcd.drawCircle(x, y, CIRCLE_RADIUS, colour);
	lcd.drawLine(x - CIRCLE_RADIUS, y, x + CIRCLE_RADIUS, y, colour);
	lcd.drawLine(x, y - CIRCLE_RADIUS, x, y + CIRCLE_RADIUS, colour);
}

/*
 * Draw a cursor on the screen
 */
void drawCursorOnScreen(int16_t accelX, int16_t accelY){
	//erase the old cursor
	drawCursor(xOld, yOld, BACKGROUND_COLOUR);

	//Calculate the coordinates to draw the cursor
	y = LCD_X_MIN + CIRCLE_RADIUS + ((+accelX + ACCELEROMETER_RANGE) *
    (LCD_WIDTH-2*CIRCLE_RADIUS) / (2 * ACCELEROMETER_RANGE));
	x = LCD_Y_MIN + CIRCLE_RADIUS + ((-accelY + ACCELEROMETER_RANGE) *
    (LCD_HEIGHT-2*CIRCLE_RADIUS) / (2 * ACCELEROMETER_RANGE));

	//Draw axis
	lcd.drawLine(0,LCD_HEIGHT/2,LCD_WIDTH,LCD_HEIGHT/2,FOREGROUND_COLOUR);
	lcd.drawLine(LCD_WIDTH/2,0,LCD_WIDTH/2,LCD_HEIGHT,FOREGROUND_COLOUR);

	//Draw cursor
	drawCursor(x, y, FOREGROUND_COLOUR);

	//store the coordinates to erase this circle next call
	xOld = x;
	yOld = y;
}
