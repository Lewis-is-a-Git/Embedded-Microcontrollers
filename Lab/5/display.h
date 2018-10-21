/*
 * display.h
 * This module is the LCD interface, plots the captured values to the screen.
 */
#ifndef SOURCES_DISPLAY_H_
#define SOURCES_DISPLAY_H_
#include "stdint.h" //uint8_t

/** Number of data points to capture - Adjust as needed */
constexpr unsigned DATA_POINTS = 100;

/**
 * Initalise the display, clears the screen and set the font
 */
void displayInit();

/**
 * Draws the menu on the screen
 * @param[in] menuChoice - menu item index
 */
void drawMenu(int menuChoice);

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
 * Draw a sampling animation
 */
void drawSampling();

#endif /* SOURCES_DISPLAY_H_ */
