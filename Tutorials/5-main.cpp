/*
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Basic C++ demo
 *
 *  Created on: 6/9/18
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

/**
 * See more examples in Snippets directory
 */

// LED connection - change as required
using Led   = GpioC<0, ActiveLow>;
using Button = GpioC<1, ActiveLow>;

int main() {

   Led::setOutput();
   Button::setInput(PinPull_Up);
   bool lastSwitchValue = false;
   bool currentSwitchValue = false;

   for(int count = 0;;count++) {
      currentSwitchValue = Button::isPressed();

      if (!lastSwitchValue && currentSwitchValue){
    	  Led::toggle();
      }

      lastSwitchValue = currentSwitchValue;
   }
   return 0;
}
