/*
 * capture.cpp
 *
 *  Created on: 17 Oct 2018
 *      Author: FAHAD
 */

#include "capture.h"
#include "hardware.h"
#include "display.h"
#include "pit.h"
#include "stdint.h"

using namespace USBDM;

// Connection mapping - change as required
using adc = Adc0;

// Jog switch on LCD board
using EastSwitch   = USBDM::GpioC<0,ActiveLow>;
using SouthSwitch  = USBDM::GpioC<1,ActiveLow>;
using WestSwitch   = USBDM::GpioD<6,ActiveLow>;
using CentreSwitch = USBDM::GpioD<5,ActiveLow>;
using NorthSwitch  = USBDM::GpioB<1,ActiveLow>;

using potchannel = Adc0Channel<12>;

using vdd = GpioA<13>;
using ground = GpioC<10>;

using timer = Pit;
using timerChannel = PitChannel<0>;

int dataCounter = 0;
uint8_t data[DATA_POINTS];

static constexpr unsigned MAX_DATA_VALUE = 110;

void joystickInit(){

	EastSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	SouthSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	WestSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	CentreSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	NorthSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
}

void readJoyStick(){
	static int menuChoiceIndex = 0;
	int rates[] = {1, 2, 5, 10, 20, 50, 100, 200, 500};

	int currentSwitchValue =
			(EastSwitch::read()<<4)|
			(SouthSwitch::read()<<3)|
			(WestSwitch::read()<<2)|
			(CentreSwitch::read()<<1)|
			(NorthSwitch::read()<<0);

	int oldSwitchValue;
	//up = 1
	//in = 2
	//left = 4
	//down = 8
	//right = 16

	//move up
	if (currentSwitchValue == 1 && oldSwitchValue == 0){
		menuChoiceIndex++;
		if (menuChoiceIndex > 8){
			menuChoiceIndex = 8;
		}
		drawMenu(menuChoiceIndex);
	}
	//move down
	if (currentSwitchValue == 8 && oldSwitchValue == 0){
		menuChoiceIndex--;
		if (menuChoiceIndex < 0){
			menuChoiceIndex = 0;
		}
		drawMenu(menuChoiceIndex);
	}
	if (currentSwitchValue == 2){
		captureInit();
		capture(rates[menuChoiceIndex], data);
	}

	oldSwitchValue = currentSwitchValue;
}

void capture(int rate, uint8_t array[DATA_POINTS]){
	timerChannel::configure(rate*ms, PitChannelIrq_Enable);
	timerChannel::enableNvicInterrupts(true, NvicPriority_Normal);
}

bool isCaptureComplete(){
	return (dataCounter > 98);
}


void PitCallback(void){
	adc::enableNvicInterrupts(true, NvicPriority_Normal);
	potchannel::startConversion(AdcInterrupt_enable);
}
void adcCallback(uint32_t value, int channel){
	DrawSampling();
	data[dataCounter] = value*5;
	if (data[dataCounter] > MAX_DATA_VALUE){
		data[dataCounter] = MAX_DATA_VALUE;
	}
	console.write("data value = ").writeln(data[dataCounter]);
	dataCounter++;
	adc::enableNvicInterrupts(false, NvicPriority_Normal);
	if (isCaptureComplete()){
		timerChannel::enableNvicInterrupts(false, NvicPriority_Normal);
	}

}

void  captureInit(){
	adc::configure(AdcResolution_12bit_se, AdcClockSource_Bus, AdcClockDivider_2 );
	adc::calibrate();
	adc::setCallback(adcCallback);
	potchannel::setInput();

	potchannel::startConversion(AdcInterrupt_enable);

	vdd::setOut();
	vdd::write(1);

	ground::setOutput();
	ground::write(0);

	timer::configure(PitDebugMode_Stop);
	timerChannel::setCallback(PitCallback);
	checkError();
}
void displayData(){
	displayInit();
	drawGrid();
	drawData(data);
}

