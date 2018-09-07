/**
 * @file T5.PIT_Skeleton.cpp
 */
#include "hardware.h"
#include "pit.h"

using namespace USBDM;

/**
 * This example uses PIT interrupts.
 *
 * It is necessary to enable these in Configure.usbdmProject
 * under the "Peripheral Parameters"->PIT tab.
 * Select irqHandlerChannelX option (Class Method - Software ...)
 */

// Connection mapping - change as required
using Switch = USBDM::GpioC<1,ActiveLow>;  // Arduino A1
using Led    = USBDM::GpioC<0,ActiveHigh>; // Arduino A0

// Time between samples of the input
static constexpr float DEBOUNCE_SAMPLE_TIME = 5*ms;

// Number of debug samples for a stable input
static constexpr unsigned DEBOUNCE_SAMPLES = 5;

// Debounced switch value
static volatile bool switchValue = false;

// PIT & channel to use
using Timer        = Pit;
using TimerChannel = PitChannel<0>;

/**
 * This callback is set programmatically
 *
 * It polls the switch and provides a debounced result in switchValue
 */
void deBouncer() {
   static int  stableCount     = 0;
   static bool lastSwitchValue = false;

   bool currentSwitchValue = Switch::isPressed();

   // Your code here
   if (lastSwitchValue == currentSwitchValue) {
	   stableCount ++;
   }
   else {
	   stableCount = 0;
   }

   if (stableCount == 5){
	   switchValue = currentSwitchValue;
   }

   lastSwitchValue = currentSwitchValue;
}

int main() {
   Switch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
   Led::setOutput(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

   // PIT configure
   Timer::configure(PitDebugMode_Stop);

   // Set handler for PIT channel programmatically
   TimerChannel::setCallback(deBouncer);

   // Setup de-bouncer to execute @PIT_FREQUENCY
   TimerChannel::configure(DEBOUNCE_SAMPLE_TIME, PitChannelIrq_Enable);

   // Enable interrupts on the channel
   TimerChannel::enableNvicInterrupts(true, NvicPriority_Normal);

   // Check for errors so far
   checkError();

   for(;;) {
      static bool lastSwitchValue = false;

      bool currentSwitchValue = switchValue;
	  // Change condition as required
      if (!lastSwitchValue && currentSwitchValue) {
         Led::toggle();
      }
      lastSwitchValue = currentSwitchValue;
   }
}
