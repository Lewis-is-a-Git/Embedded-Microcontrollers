/*
 * display.h
 *
 *  Created on: 17 Oct 2018
 *      Author: FAHAD
 */

#ifndef SOURCES_DISPLAY_H_
#define SOURCES_DISPLAY_H_
#include "stdint.h"

/** Number of data points to capture - Adjust as needed */
constexpr unsigned DATA_POINTS = 100;


/**
* Draws data on screen (as fast as possible!)
*
* @param data - Array of data to plot
*/
void drawData(uint8_t array[DATA_POINTS]);
/**
* Draw grid and axis labels for display
*/
void drawGrid();
/**
* Initialise LCD
*/

void drawMenu(int menuChoice);

void displayInit();


void DrawSampling();


#endif /* SOURCES_DISPLAY_H_ */
