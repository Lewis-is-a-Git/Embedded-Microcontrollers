/*
 * capture.h
 * This module controls the capture functions of the program
 * Reads the joystick inputs to control the menu
 */

#ifndef SOURCES_CAPTURE_H_
#define SOURCES_CAPTURE_H_

#include "stdint.h"
#include "display.h"

/**
 * Initailise the joystick controls
 */
void joystickInit();

/**
 * Read Joystick and Control Menu Selection
 */
void readJoyStick();

/**
 * Start capture of data values at a fixed rate.
 * This routine may return before the data
 * capture is complete.
 *
 * @param[in] rate Sample rate (1 to 500 ms)
 * @param[out] data Buffer for data
 *
 */
void capture(int rate, uint8_t array[DATA_POINTS]);
/**
 * Checks if capture is complete
 *
 * @return true => capture complete
 * @return false => capture busy
 */
bool isCaptureComplete();

/**
 * Initialise the ADC and PIT to capture data
 */
void captureInit();

/**
 * Displays the Data after capture is complete
 */
void displayData();

#endif /* SOURCES_CAPTURE_H_ */
