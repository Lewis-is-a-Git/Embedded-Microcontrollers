/*
 * display.cpp
 *
 */
#include "display.h"
#include "hardware.h"
#include "spi.h"
#include "lcd.h"
#include "capture.h"

using namespace USBDM;
// SPI interface
Spi0 spi;

// LCD interface using SPI
Lcd lcd(spi);

/// LCD derived dimensions
static constexpr int LCD_WIDTH  = (LCD_X_MAX-LCD_X_MIN);
static constexpr int LCD_HEIGHT = (LCD_Y_MAX-LCD_Y_MIN);
static constexpr int CENTRE_X   = ((LCD_X_MAX-LCD_X_MIN)/2);
static constexpr int CENTRE_Y   = ((LCD_Y_MAX-LCD_Y_MIN)/2);

//grid values
static constexpr unsigned GRID_OFFSET = 19;
static constexpr unsigned GRID_LINES = 6;
static constexpr unsigned GRID_X_INCREMENT = (LCD_WIDTH - GRID_OFFSET) / GRID_LINES;
static constexpr unsigned GRID_Y_INCREMENT = (LCD_HEIGHT - GRID_OFFSET) / GRID_LINES;

// Colours for LCD
static constexpr Colour BACKGROUND_COLOUR = (BLACK);
static constexpr Colour FOREGROUND_COLOUR = (WHITE);
static constexpr Colour DATA_COLOUR = (RED);
static constexpr Colour GRID_COLOUR= (BLACK);

/**
 * Initalise the display, clears the screen and set the font
 */
void displayInit(){
	lcd.clear(BACKGROUND_COLOUR);
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);
}

/**
 * Draws the menu on the screen
 * @param[in] menuChoice - menu item index
 */
void drawMenu(int menuChoice){
	lcd.putStr("Rate = ", 10, CENTRE_Y+30, FOREGROUND_COLOUR, BACKGROUND_COLOUR);

	//array of rate lables, must match the rates array in readjoystick()
	char* rates[] = {"1ms  ", "2ms  ", "5ms  ", "10ms ", "20ms ", "50ms ", "100ms", "200ms", "500ms"};
	lcd.putStr(rates[menuChoice], 20, CENTRE_Y, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
}


/**
 * Draws data on screen (as fast as possible!)
 *
 * @param data - Array of data to plot
 */
void drawData(uint8_t data[DATA_POINTS]){
	//loops through the array plotting on the screen
	for (unsigned dataIndex = 0; dataIndex < DATA_POINTS - 1; dataIndex++){
		// draw a line between the current point in the array and the previous point
		if (dataIndex > 0){
			lcd.drawLine((dataIndex - 1) + GRID_OFFSET, data[dataIndex - 1] + GRID_OFFSET, (dataIndex) + GRID_OFFSET, data[dataIndex] + GRID_OFFSET, DATA_COLOUR);
		}
		// except if it is the first point in the array just draw a dot
		else {
			lcd.drawPixel(dataIndex + GRID_OFFSET, data[dataIndex] + GRID_OFFSET, DATA_COLOUR);
		}
	}
}

/**
 * Draw grid and axis labels for display
 */
void drawGrid(){
	for (unsigned i = 0; i < GRID_LINES; i++){
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
	lcd.putStr("0", 9, 16, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("2", 9, 34, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("4", 9, 53, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("6", 9, 71, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("8", 9, 89, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("10", 5, 106,FOREGROUND_COLOUR, BACKGROUND_COLOUR);

	//x axis labels
	// 100ms * 100 data points is 10,000ms or 10 seconds
	lcd.putStr("0", 17, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("2", 35, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("4", 53, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("6", 71, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("8", 89, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("10", 106, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
}

/**
 * Draw a sampling animation
 */
void drawSampling(){
	static int animationIndex = 0;
	char* animation[] = {"sampling   ", "sampling.  ", "sampling.. ", "sampling..."};
	lcd.putStr(animation[animationIndex], CENTRE_X-30, CENTRE_Y-30,
			FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	animationIndex++;
	//dont go over the end of the array
	if (animationIndex > sizeof(animation)/sizeof(animation[0])){
		animationIndex = 0;
		//clear the animation
		lcd.putStr(animation[0], CENTRE_X-30, CENTRE_Y-30,
					FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	}

}
