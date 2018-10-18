/*
 * display.cpp
 *
 *  Created on: 17 Oct 2018
 *      Author: FAHAD
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

// Colour for LCD background
static constexpr int BACKGROUND_COLOUR = (BLACK);

// Colour for LCD foreground
static constexpr int FOREGROUND_COLOUR = (WHITE);

static constexpr Colour DATA_COLOUR = (RED);
static constexpr Colour GRID_COLOUR= (BLACK);

void drawMenu(int menuChoice){

	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);
	lcd.clear(BACKGROUND_COLOUR);
	lcd.putStr("Rate = ", 10, CENTRE_Y+30, FOREGROUND_COLOUR, BACKGROUND_COLOUR);

	char* rates[] = {"1ms", "2ms", "5ms", "10ms", "20ms", "50ms", "100ms", "200ms", "500ms"};

	lcd.putStr(rates[menuChoice], 20, CENTRE_Y, FOREGROUND_COLOUR, BACKGROUND_COLOUR);

}


void displayInit(){
	lcd.clear(BACKGROUND_COLOUR);
	lcd.setFont(smallFont).setForeground(FOREGROUND_COLOUR).setBackground(BACKGROUND_COLOUR);
}

void drawData(uint8_t data[DATA_POINTS]){
	console.writeln("start of drawdata");
	for (unsigned dataIndex = 0; dataIndex < DATA_POINTS; dataIndex++){
		lcd.drawPixel(dataIndex + GRID_OFFSET, data[dataIndex] + GRID_OFFSET, DATA_COLOUR);
		if (dataIndex > 0){
			lcd.drawLine((dataIndex - 1) + GRID_OFFSET, data[dataIndex - 1] + GRID_OFFSET, (dataIndex) + GRID_OFFSET, data[dataIndex] + GRID_OFFSET, DATA_COLOUR);
		}
		dataIndex++;
		if (dataIndex == DATA_POINTS){dataIndex = 0;}
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
	//covert char to int
	lcd.putStr("0", 17, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("2", 35, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("4", 53, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("6", 71, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("8", 89, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("10", 106, 7, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
}

void DrawSampling(){
	lcd.putStr("sampling .", CENTRE_X-30, CENTRE_Y-30, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("sampling ..", CENTRE_X-30, CENTRE_Y-30, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("sampling ...", CENTRE_X-30, CENTRE_Y-30, FOREGROUND_COLOUR, BACKGROUND_COLOUR);
	lcd.putStr("sampling ...", CENTRE_X-30, CENTRE_Y-30, BACKGROUND_COLOUR, BACKGROUND_COLOUR);


}


