/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 10/1/2016
 *      Author: podonoghue
 ============================================================================
 */
#include "hardware.h"
#include "screen.h"
// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

int main() {
	displayInit();

   for(;;) {

      drawGrid();
      waitMS(100);
   }
   return 0;
}
