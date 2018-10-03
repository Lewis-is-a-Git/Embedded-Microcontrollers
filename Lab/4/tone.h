/**
 * tone.h
 *
 *  An interrupt driven system for playing
 *  a single continuous note
 */
#ifndef SOURCES_NOTE_H_
#define SOURCES_NOTE_H_

#include "stdint.h" //int16_t

/** Maximum note frequency */
constexpr int NOTE_MAX = 20000;

/** Minimum note frequency */
constexpr int NOTE_MIN = 1000;

/**
 * Initialise the note playing system before first use
 */
void initialiseNotes(void);

/**
 *  Calculate the frequency based on accelerometer data
 *  @param accelX - X axis of accelerometer
 *  @param accelY - Y axis of accelerometer
 */
int calculateFrequency(int16_t accelX, int16_t accelY);

/**
 * Changes the frequency of the note being played
 *
 * @param frequency - Frequency of note (100Hz-10kHz)
 *
 * @note This function returns immediately.
 *       The note is played by interrupt driven code.
 */
void setNoteFrequency(unsigned frequency);

#endif /* SOURCES_NOTE_H_ */
