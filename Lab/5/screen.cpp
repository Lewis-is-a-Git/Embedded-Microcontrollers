/*
 * screen.cpp
 *
 *  Created on: 3 Oct 2018
 *      Author: root
 */
#include "screen.h"

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

// LCD derived dimensions
static constexpr unsigned LCD_WIDTH  = (LCD_X_MAX-LCD_X_MIN);
static constexpr unsigned LCD_HEIGHT = (LCD_Y_MAX-LCD_Y_MIN);
static constexpr unsigned CENTRE_X   = ((LCD_X_MAX-LCD_X_MIN)/2);
static constexpr unsigned CENTRE_Y   = ((LCD_Y_MAX-LCD_Y_MIN)/2);

// Colour for LCD background
static constexpr Colour BACKGROUND_COLOUR = (WHITE);

// Colour for LCD foreground
static constexpr Colour FOREGROUND_COLOUR = (BLUE);

// Radius used for the moving circle
static constexpr unsigned CIRCLE_RADIUS = (20);

/**
 * Draws data on screen (as fast as possible!)
 *
 * @param data - Array of data to plot
 */
void drawData(uint8_t data[DATA_POINTS]){

}
/**
 * Draw grid and axis labels for display
 */
void drawGrid(){
	int xInc, yInc;
	xInc = LCD_WIDTH/5;
	yInc = LCD_HEIGHT/5;


	for (int i = 0; i < 6; i++){
		//verticle lines
		lcd.drawLine(xInc * i + 10, LCD_HEIGHT, xInc * i + 10, 0, FOREGROUND_COLOUR);
		//horizonatal Lines
		lcd.drawLine(0, yInc * i + 10, LCD_WIDTH, yInc * i + 10, FOREGROUND_COLOUR);
		//lable backgrounds verticle
		lcd.drawRect(0, 0, 9, LCD_HEIGHT, 1, BACKGROUND_COLOUR);
		//lable backgrounds horizontal
		lcd.drawRect(0, 0, LCD_WIDTH, 9, 1, BACKGROUND_COLOUR);

		//y axis lables
		for (int j = 0; j < 6; j++){
			lcd.putChar(2 * j, 5, yInc * i + 10, mediumFont, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
		}


	}
	console.write(xInc).write(" ").writeln(yInc);
	//	lcd.putChar();
}
/**
 * Initialise LCD
 */
void displayInit(){
	lcd.clear(BACKGROUND_COLOUR);
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);


}
