/*
 ============================================================================
 * @file    main.cpp
 * @brief   Magnetic pulse gun, fire a pellet based on timing delays
 *
 *  Created on: 27/9/18
 *      Author: lewis
 ============================================================================
 */
#include "hardware.h"

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

//Fire Button
using Button   = GpioB<1,ActiveLow>;

// Field for the Coil pins
using CoilDrivers = GpioCField<2,0>;

// Timer
using Timer = Ftm0;

// Timer channel for output
using TimerChannel = Ftm0Channel<5>;

/*
 * Structure to describe coil gun events
 */
struct CoilGunData {
	uint16_t deltaTime;  // Time to next event in ticks
	uint32_t pinMask;    // Coil output values
};

/*
 * Data for coil gun events
 */
static CoilGunData data[6];

/*
 * Event counter.  Used to index coil data.
 * Must be volatile as it is set by the call-back
 * and polled by the main-line code.
 */
static volatile unsigned eventCounter;

// Map coils to pins in field
static constexpr uint32_t COIL_1 = (1<<0);
static constexpr uint32_t COIL_2 = (1<<1);
static constexpr uint32_t COIL_3 = (1<<2);

// Maximum duration to measure
// Must be largest of pulse widths or inter-pulse times
static constexpr float MAX_INTERVAL = 20*ms;

/**
 * Set up coil data
 *
 * @note Must be done after the Timer is configured!
 */
void initCoilGunData() {
	/* This is a little ugly but necessary because:
	 * - I do not want to do calculations in the interrupt handler (FTM callback)
	 * - I can't calculate the values as constants as they depend on the FTM prescaler
	 */
	int i = 0;
	data[i].deltaTime = Timer::convertMicrosecondsToTicks(3000);  data[i++].pinMask = COIL_1;
	data[i].deltaTime = Timer::convertMicrosecondsToTicks(20000); data[i++].pinMask = 0;
	data[i].deltaTime = Timer::convertMicrosecondsToTicks(2000);  data[i++].pinMask = COIL_2;
	data[i].deltaTime = Timer::convertMicrosecondsToTicks(15000); data[i++].pinMask = 0;
	data[i].deltaTime = Timer::convertMicrosecondsToTicks(1000);  data[i++].pinMask = COIL_3;
	data[i].deltaTime = 0;                                        data[i++].pinMask = 0;

	// Check for underflow/overflow in convertMicrosecondsToTicks()
	checkError();
}

/**
 * Set up coil data (Version 2)
 * This version sets up the global array from a
 * static const array which will be in ROM.
 * A bit more practical if the sequence is longer.
 *
 * @note Must be done after the Timer is configured!
 */
void initCoilGunDataVer2() {
	/* This is a little ugly but necessary because:
	 * - I do not want to do calculations in the interrupt handler (FTM callback)
	 * - I can't calculate the values as constants as they depend on the FTM prescaler
	 */
	static const CoilGunData initData[] {
			{ 3000, COIL_1},
			{20000, 0     },
			{ 2000, COIL_2},
			{15000, 0     },
			{ 1000, COIL_3},
			{    0, 0     }, // sentinel
	};
	for (int i=0;;i++) {
		data[i].deltaTime = Timer::convertMicrosecondsToTicks(initData[i].deltaTime);
		data[i].pinMask   = initData[i].pinMask;
		if (initData[i].deltaTime == 0) {
			break;
		}
	}
	// Check for underflow/overflow in convertMicrosecondsToTicks()
	checkError();
}

/**
 * Interrupt handler for Timer interrupts
 * - Sets the next interrupt relative to the last event
 * - Changes the coil outputs
 * - On last event it disables the timer
 *
 * @param[in] status Flags indicating interrupt source channel(s)
 */
static void ftmCallback(uint8_t status) {

	// Check channel
	if (status & TimerChannel::CHANNEL_MASK) {
		// Changes the coil outputs
		CoilDrivers::write(data[eventCounter].pinMask);

		if (data[eventCounter].deltaTime == 0) {
			// On last event disable timer channel
			TimerChannel::enableInterrupts(false);
			TimerChannel::setMode(FtmChMode_Disabled);
		}
		else {
			// Sets the next interrupt relative to the last event
			TimerChannel::setDeltaEventTime(data[eventCounter].deltaTime);
		}
		eventCounter++;
	}
}

/**
 * Initialise the Coil Gun before 1st use
 */
void initCoilGun() {

	// Initialise the output pins
	CoilDrivers::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Fast);

	// Configure FTM (affects all channels)
	Timer::configure(
			FtmMode_LeftAlign,      // Left-aligned is required for OC/IC
			FtmClockSource_System); // Bus clock usually

	// Set OC measurement period to accommodate maximum period + 10%
	// This adjusts the prescaler value but does not change the clock source
	Timer::setMeasurementPeriod(1.1*MAX_INTERVAL);

	// Set up event data
	initCoilGunData();

	// Set callback function
	Timer::setChannelCallback(ftmCallback);

	// Enable interrupts for entire timer
	Timer::enableNvicInterrupts();

	// Check if configuration failed
	USBDM::checkError();
}

/**
 * Fires the coil gun
 *
 * @note Doesn't return until the sequence is complete
 */
void fireCoilGun() {

	// Start events from 0;
	eventCounter = 0;

	// Trigger 1st interrupt at now+100
	TimerChannel::setRelativeEventTime(100);

	// Configure the channel as output compare
	TimerChannel::configure(
			FtmChMode_OutputCompare, //  Output Compare without pin
			FtmChannelAction_Irq);   //  + interrupts on events

	while (TimerChannel::getMode() != FtmChMode_Disabled) {
		__asm__("nop");
	}
	// Wait for sequence to complete
	while (eventCounter<6) {
		__asm__("nop");
	}
}

int main() {
	//few gotchas
	//the intention is not to use 3 timer channels
	//easier to use the software
	//generate interrupts
	//6 actions, each change is an event

	//the ftm uses a counter
	//the counter counts
	//output compare mode

	//counter automatically
	//runs callback
	//changes a pin

	//callback
	//sets the next event
	//use a counter to count hte events
	//keep track of shish step we are up to in the sequence

	//after, stop the ftm
	//we dont want anymore events

	//use table driven
	//case statemetns is acceptable for exam

	Button::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	initCoilGun();

	while(true){
		//wait for button press
		while(Buttin::isInactive()){}
		
		fireCoilGun();
		//wait for button release
		while(Button::isActive()){}
		
		//Debounce Delay
		waitMS(50);
	}

	return 0;
}
