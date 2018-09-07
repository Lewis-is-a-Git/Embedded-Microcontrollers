/**
 ============================================================================
 * @file  pit-example2.cpp (180.ARM_Peripherals/Snippets/pit-example3-MK.cpp)
 * @brief Programmable Interrupt Timer (PIT) Example
 * @author   podonoghue
============================================================================
 */
#include "hardware.h"
#include "pit.h"

using namespace USBDM;

/**
 * Programmable Interrupt Timer (PIT) Example
 *
 * Demonstrates PIT call-back
 *
 * Toggles LED
 */
/**
 * This example uses PIT interrupts.
 *
 * It is necessary to enable these in Configure.usbdmProject
 * under the "Peripheral Parameters"->PIT tab.
 * Select irqHandlerChannelX option (Class Method - Software ...)
 */

// Connection mapping - change as required
using Led = GpioC<0,ActiveLow>;

using Timer        = Pit; //programmable interupt timer
using TimerChannel = PitChannel<0>;
/*
 * This callback is set programmatically
 */
void flash(void) {
   Led::toggle();
}

int main() {
   Led::setOutput(
         PinDriveStrength_High,
         PinDriveMode_PushPull,
         PinSlewRate_Slow);

   Timer::configure(PitDebugMode_Stop);

   // Set handler programmatically
   TimerChannel::setCallback(flash); //setup the interupt to call the function flash wait for trigger

   // Flash LED @ 1Hz, Because it toggles twice a second
//   TimerChannel::configureInTicks(::SystemBusClock/2, PitChannelIrq_Enable);
   TimerChannel::configure(0.5 * seconds, PitChannelIrq_Enable); //allow interupts

   TimerChannel::enableNvicInterrupts(true, NvicPriority_Normal); //enable interupts on nvic

   // Check for errors so far
   checkError(); //checks the sticky error flag, (numbers are too big for the timer)

   for(;;) {
      // Sleep between interrupts
      __asm__("wfi");
   }
}
