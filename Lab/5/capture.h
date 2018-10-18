/*
 * capture.h
 *
 *  Created on: 17 Oct 2018
 *      Author: FAHAD
 */

#ifndef SOURCES_CAPTURE_H_
#define SOURCES_CAPTURE_H_

#include "stdint.h"
#include "display.h"

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

void captureInit();

void joystickInit();

void readJoyStick();

void displayData();

#endif /* SOURCES_CAPTURE_H_ */
