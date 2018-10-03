/*
 * level.h
 * Interface for lCD display
 */

#ifndef SOURCES_LEVEL_H_
#define SOURCES_LEVEL_H_
#include "stdint.h" //int16_t

/*
 * Initailise the LCD level Display
 */
void InitialiseLevel();

/*
 * Draw a cursor on the screen
 */
void drawCursorOnScreen(int16_t accelX, int16_t accelY);

#endif /* SOURCES_LEVEL_H_ */
