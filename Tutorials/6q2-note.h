/**
 * Note.h
 *
 *  An interrupt driven system for playing
 *  a single continuous note
 */
#ifndef SOURCES_NOTE_H_
#define SOURCES_NOTE_H_

/** Maximum note frequency */
constexpr int NOTE_MAX = 20000;

/** Minimum note frequency */
constexpr int NOTE_MIN = 1000;

/**
 * Initialise the note playing system before first use
 */
void initialiseNotes(void);

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
