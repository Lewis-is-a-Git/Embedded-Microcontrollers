/*
 * capture.cpp
 *
 *  Created on: 12 Oct 2018
 *      Author: root
 */
#include "hardware.h"
#include "pit.h"

#include "capture.h"
#include "screen.h"

using namespace USBDM;

using adc = Adc0;
using potChannel = Adc0Channel<12>;
using ground = GpioB<3>;

using timer = Pit;
using timerChannel = PitChannel<0>;

//GLobal variables to communicate to callback function
uint8_t data[100];
int dataCounter=0;

/**
 * Start capture of data values at a fixed rate.
 * This routine may return before the data
 * capture is complete.
 *
 * @param[in] rate Sample rate (1 to 500 ms)
 * @param[out] data Buffer for data
 *
 */
void capture(int rate, uint8_t data[DATA_POINTS]){
	data[dataCounter] = potChannel::readAnalogue();
	timerChannel::configure(rate*ms, PitChannelIrq_Enable);
}
/**
 * Checks if capture is complete
 *
 * @return true => capture complete
 * @return false => capture busy
 */
bool isCaptureComplete(){
	return (dataCounter == 100);
}
/**
 * PIT callback function
 * captures data from the adc
 * plots the data on the screen
 * Increments data counter global variable
 */
void pitCallback(void){

	capture(100, data);
	drawData(data);

	dataCounter++;
	if (isCaptureComplete()){
		dataCounter = 0;
		displayInit();
	}
}
/**
 * Initialise the capture of the adc values using the PIT as a timer
 */
void captureInit(){
	//joystick needs 8bit (256) screen uses 12bit (4096)
	adc::configure(AdcResolution_8bit_se, AdcClockSource_Bus, AdcClockDivider_2);
	adc::calibrate();
	adc::setAveraging(AdcAveraging_32);

	potChannel::setInput();

	//simulate ground pin, because of access
	ground::setOutput();
	ground::write(0);

	timer::configure();

	timerChannel::setCallback(pitCallback);
	timerChannel::configure(100*ms, PitChannelIrq_Enable);
	timerChannel::enableNvicInterrupts(true, NvicPriority_Normal);
}
/**
 * used to pause the data capture while on the menu
 */
void startStopPit(bool start, int slowness){
	if (start){
		timerChannel::configure(slowness*ms, PitChannelIrq_Enable);
	}
	if (!start){
		timerChannel::configure(slowness*ms, PitChannelIrq_Disable);
	}
}

