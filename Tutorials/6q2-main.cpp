/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   play tone through speaker using ftm interupts
 *
 *  Created on: 20/9/18
 *      Author: lewis
 ============================================================================
 */
#include <note.h>
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

int main() {
   static const int frequencies[] = {
         50,
         100,
         (int)(1*kHz),
         (int)(2*kHz),
         (int)(5*kHz),
         (int)(10*kHz),
         (int)(20*kHz),
   };

   initialiseNotes();

   for(;;) {
	   for(unsigned t=0; t<(sizeof(frequencies)/sizeof(frequencies[0])); t++) {
         int frequency = frequencies[t];
         setNoteFrequency(frequency);
         console.write("Frequency = ").writeln(frequency);
         waitMS(100);
      }
//	   setNoteFrequency(1*kHz);
//	   waitMS(500);
//	   setNoteFrequency(10*kHz);
//	   waitMS(500);
   }
   return 0;
}
