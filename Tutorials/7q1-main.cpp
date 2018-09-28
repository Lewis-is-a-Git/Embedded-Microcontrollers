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

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

// IO
using Buttons   = GpioCField<2 ,0, ActiveLow>;
using Restart = GpioC<3, ActiveLow>;
using Lights = GpioCField<6, 4>;
using Buzzer = GpioC<7>;

//Timer
using Timer = Ftm0;
using TimerChannel = Ftm0Channel<7>;

// Period between input edges in ticks
// This variable is shared with the interrupt routine
static volatile uint16_t periodInTicks = 0;

// Maximum measurement time
static const float MEASUREMENT_TIME = 100*ms;

static void ftmCallback(uint8_t status) {
   static uint16_t risingEdgeEventTime;

   // Check channel
   if (status & TimerChannel::CHANNEL_MASK) {
      uint16_t currentEventTime = TimerChannel::getEventTime();
      periodInTicks = currentEventTime-risingEdgeEventTime;
      risingEdgeEventTime = currentEventTime;
   }
}


int main() {
	Buttons::setInput(PinPull_Up);
	Restart::setInput(PinPull_Up);
	Lights::setOutput();
	Buzzer::setOutput();

	   /**
	    * FTM channel set as Input Capture using a callback function
	    */
	   // Configure base FTM (affects all channels)
	   Timer::configure(
	         FtmMode_LeftAlign,      // Left-aligned is required for OC/IC
	         FtmClockSource_System,  // Bus clock usually
	         FtmPrescale_1);         // The prescaler will be re-calculated later

	   // Set IC/OC measurement period to accommodate maximum measurement + 10%
	   // This adjusts the prescaler value but does not change the clock source
	   Timer::setMeasurementPeriod(1.1*MEASUREMENT_TIME);

	   // Set callback function
	   Timer::setChannelCallback(ftmCallback);

	   // Enable interrupts for entire timer
	   Timer::enableNvicInterrupts();

	   // Configure pin associated with channel
	   TimerChannel::setInput(
	         PinPull_None,
	         PinAction_None,
	         PinFilter_Passive);
	   // or change individual attributes
	   //  TimerChannel::setPullDevice(PinPull_Up);
	   //  TimerChannel::setFilter(PinFilter_Passive);

	   // Configure the channel
	   TimerChannel::configure(
	         FtmChMode_InputCaptureRisingEdge, // Input capture rising edge
	         FtmChannelAction_Irq);            //  + interrupts on events

	   // Check if configuration failed
	   USBDM::checkError();

	while(true){
		//take snapshot
		int ContestantButtons = Buttons::read();
		//process snapshio

		//turn on lights

		//turn on buzzer

		//turn off buzzer

		//wait for restart
	}

	return 0;
}
