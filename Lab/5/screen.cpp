/*
 * screen.cpp
 *
 *  Created on: 3 Oct 2018
 *      Author: root
 */
#include "screen.h"

#include "lcd.h"
#include "spi.h"12.


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
// Grid values
static constexpr unsigned GRID_OFFSET  = 19;
static constexpr unsigned GRID_LINES  = 6;
static constexpr unsigned GRID_X_INCREMENT  = (LCD_WIDTH-GRID_OFFSET) / GRID_LINES;
static constexpr unsigned GRID_Y_INCREMENT  = (LCD_HEIGHT-GRID_OFFSET) / GRID_LINES;

// Color for LCD background
static constexpr Colour BACKGROUND_COLOUR = (WHITE);

// Color for LCD foreground
static constexpr Colour FOREGROUND_COLOUR = (BLUE);
//Colour for LCD Data Line
static constexpr Colour DATA_COLOUR = (RED);

/**
 * Draws data on screen (as fast as possible!)
 *
 * @param data - Array of data to plot
 */
void drawData(uint8_t data[DATA_POINTS]){
	static int i = 0;
	lcd.drawPixel(i+GRID_OFFSET, data[i]+GRID_OFFSET, DATA_COLOUR);
	if(i>0){
		lcd.drawLine((i-1)+GRID_OFFSET, data[i-1]+GRID_OFFSET, (i)+GRID_OFFSET, data[i]+GRID_OFFSET, DATA_COLOUR );
	}
	i++;
	if (i == DATA_POINTS){i = 0;}
}
/**
 * Draw grid and axis labels for display
 */
void drawGrid(){
	for (int i = 0; i < 6; i++){
		//vertical lines
		lcd.drawLine(GRID_X_INCREMENT * i + GRID_OFFSET, LCD_HEIGHT,
				GRID_X_INCREMENT * i + GRID_OFFSET, GRID_OFFSET,
				FOREGROUND_COLOUR);
		//horizontal Lines
		lcd.drawLine(GRID_OFFSET, GRID_Y_INCREMENT * i + GRID_OFFSET,
				GRID_OFFSET + DATA_POINTS, GRID_Y_INCREMENT * i + GRID_OFFSET,
				FOREGROUND_COLOUR);
	}
	//y axis labels
	lcd.putStr("0", 9, 16, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("2", 9, 34, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("4", 9, 53, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("6", 9, 71, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("8", 9, 89, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("10", 5, 106, BLACK, BACKGROUND_COLOUR);

	//x axis labels
	lcd.putStr("0", 17, 7, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("2", 35, 7, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("4", 53, 7, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("6", 71, 7, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("8", 89, 7, BLACK, BACKGROUND_COLOUR);
	lcd.putStr("10", 106, 7, BLACK, BACKGROUND_COLOUR);

}

/**
 * Initialise LCD
 */
void displayInit(){
	lcd.clear(BACKGROUND_COLOUR);
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);
	drawGrid();
}
/*
 * Display menu
 *
 * 0 means clear menu
 * 1 for option 1
 * 2 for option 2
 * 3 for option 3
 */
void displayMenu(int selection){
	displayInit();
	if (selection > 0){ //if we want to display a menu item
		//draw the three menu item choices
		//broken joystick has restricted menu design
		for (int i = 0; i < 3; i++){
			if (selection - 1 == i){
				lcd.drawRect(LCD_WIDTH/3 * i, LCD_Y_MAX-2*(largeFont.height-1), LCD_X_MAX/3 * (i+1), LCD_Y_MAX, 1, BLUE);
			}
			else {
				lcd.drawRect(LCD_WIDTH/3 * i, LCD_Y_MAX-2*(largeFont.height-1), LCD_X_MAX/3 * (i+1), LCD_Y_MAX, 1, WHITE);
			}
			//change colour of selected item
			if (selection - 1 == i){
				lcd.setForeground(WHITE);
				lcd.setBackground(BLUE);
			}
			//change the font of the selected item
			if (i == 0){
				lcd.moveXY(5+(i*LCD_WIDTH/3), (LCD_Y_MAX - (largeFont.height-1))).write("100ms");
			}
			if (i == 1){
				lcd.moveXY(5+(i*LCD_WIDTH/3), (LCD_Y_MAX - (largeFont.height-1))).write("200ms");
			}
			if (i == 2){
				lcd.moveXY(5+(i*LCD_WIDTH/3), (LCD_Y_MAX - (largeFont.height-1))).write("500ms");
			}
			//restore the colours
			lcd.setForeground(FOREGROUND_COLOUR);
			lcd.setBackground(BACKGROUND_COLOUR);
		}
	}
}
