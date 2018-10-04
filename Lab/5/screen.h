/*
 * screen.h
 *
 *  Created on: 3 Oct 2018
 *      Author: root
 */

#ifndef SOURCES_SCREEN_H_
#define SOURCES_SCREEN_H_

#include "stdint.h" //int16_t

/** Number of data points to capture - Adjust as needed */
constexpr unsigned DATA_POINTS = 100;
/**
* Draws data on screen (as fast as possible!)
*
* @param data - Array of data to plot
*/
void drawData(uint8_t data[DATA_POINTS]);
/**
* Draw grid and axis labels for display
*/
void drawGrid();
/**
* Initialise LCD
*/
void displayInit();



#endif /* SOURCES_SCREEN_H_ */
