#include <stdlib.h>
#include "lcd.h"
#include "spi.h"
#include "delay.h"

// Access to USBDM name-space
using namespace USBDM;

/* **************************************************
 *  Globally shared objects representing hardware
 * ************************************************ */

// SPI interface
Spi0 spi;

// LCD interface using SPI
Lcd lcd(spi);

/* ************************************************** */

using Adc = Adc0;
using AdcChannelX = Adc0Channel<15>;
using AdcChannelY = Adc0Channel<14>;

// LCD derived dimensions
static constexpr unsigned LCD_WIDTH  = (LCD_X_MAX-LCD_X_MIN);
static constexpr unsigned LCD_HEIGHT = (LCD_Y_MAX-LCD_Y_MIN);
static constexpr unsigned CENTRE_X   = ((LCD_X_MAX-LCD_X_MIN)/2);
static constexpr unsigned CENTRE_Y   = ((LCD_Y_MAX-LCD_Y_MIN)/2);

// Colour for LCD background
static constexpr Colour BACKGROUND_COLOUR = (RED);

// Colour for LCD foreground
static constexpr Colour FOREGROUND_COLOUR = (WHITE);

// Radius used for the moving circle
static constexpr unsigned CIRCLE_RADIUS = (20);

/*
 * Draws a cursor on the lcd screen
 *
 * @param x       x position
 * @param y       y position
 * @param colour  Colour of cursor
 *
 * @note Done this way so a more sophisticated cursor can be added
 */
void drawCursor(unsigned x, unsigned y, Colour colour) {
	lcd.drawCircle(x, y, CIRCLE_RADIUS, colour);
	//   lcd.drawRect(x-CIRCLE_RADIUS/2, y-1, x+CIRCLE_RADIUS/2, y+1, false, colour);
	//   lcd.drawRect(x-1, y-CIRCLE_RADIUS/2, x+1, y+CIRCLE_RADIUS/2, false, colour);
}

void initAdc(){
	Adc::configure(AdcResolution_12bit_se, AdcClockSource_Bus, AdcClockDivider_2);
	Adc::calibrate();
	Adc::setAveraging(AdcAveraging_4);
}

int main() {

	// Draw pretty pattern
	lcd.clear(BACKGROUND_COLOUR);
	lcd.drawCircle(CENTRE_X, CENTRE_Y, 20, WHITE);
	lcd.drawCircle(CENTRE_X, CENTRE_Y, 30, WHITE);
	lcd.drawCircle(CENTRE_X, CENTRE_Y, 40, WHITE);

	// Set LCD defaults
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);

	// Simple text with position and colours
	lcd.putStr("Some Circles", 30, 10);

	// Cursor position on screen
	unsigned x=0, y=0;
	unsigned xOld=50, yOld=50;

	// Set LCD defaults
	lcd.setFont(largeFont).setForeground(BLUE).setBackground(WHITE);

	// Formatted write to LCD (using defaults)
	lcd.moveXY(10, LCD_Y_MAX-largeFont.height-1).write("max-X=").write(LCD_X_MAX).write(" ");

	initAdc();

	for(;;) {

		x = LCD_X_MIN + CIRCLE_RADIUS + (AdcChannelX::readAnalogue() * (LCD_WIDTH-2*CIRCLE_RADIUS) / 4095);
		y = LCD_Y_MIN + CIRCLE_RADIUS + (AdcChannelY::readAnalogue() * (LCD_HEIGHT-2*CIRCLE_RADIUS) / 4095);

		console.write("x = ").write(x).write(" y = ").writeln(y);
		waitMS(100);

		drawCursor(xOld, yOld, BACKGROUND_COLOUR);
		drawCursor(x, y, FOREGROUND_COLOUR);
		xOld = x;
		yOld = y;
	}
}
