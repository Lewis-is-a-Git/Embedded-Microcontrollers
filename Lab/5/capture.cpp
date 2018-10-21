/*
 * capture.cpp
 */
#include "capture.h"
#include "hardware.h"
#include "display.h"
#include "pit.h"
#include "stdint.h"

using namespace USBDM;

// Jog switch on LCD board
using EastSwitch   = USBDM::GpioC<0,ActiveLow>;
using SouthSwitch  = USBDM::GpioC<1,ActiveLow>;
using WestSwitch   = USBDM::GpioD<6,ActiveLow>;
using CentreSwitch = USBDM::GpioD<5,ActiveLow>;
using NorthSwitch  = USBDM::GpioB<1,ActiveLow>;

// Connection mapping
using adc = Adc0;
using potchannel = Adc0Channel<12>;

//simulate pins
using vdd = GpioA<13>;
using ground = GpioC<10>;

using timer = Pit;
using timerChannel = PitChannel<0>;

//Capture counter
int dataCounter = 0;
//Data Array for ADC capture
uint8_t data[DATA_POINTS];

//maximum value that can be displayed on the LCD
static constexpr unsigned MAX_DATA_VALUE = 110;

/**
 * Initailise the joystick controls
 */
void joystickInit(){
	EastSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	SouthSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	WestSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	CentreSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
	NorthSwitch::setInput(PinPull_Up, PinAction_None, PinFilter_Passive);
}

/**
 * Read Joystick and Control Menu Selection
 */
void readJoyStick(){
	//menu options
	static int menuChoiceIndex = 0;
	int rates[] = {1, 2, 5, 10, 20, 50, 100, 200, 500};

	//read the joystick inputs
	int currentSwitchValue =
			(EastSwitch::read()<<4)|
			(SouthSwitch::read()<<3)|
			(WestSwitch::read()<<2)|
			(CentreSwitch::read()<<1)|
			(NorthSwitch::read()<<0);

	//used for rising edge detection
	int oldSwitchValue = 0;

	//Joystick Controls
	int noInput = 0;
	int up = 1;
	int press = 2;
	int down = 8;
	//int left = 4;
	//int right = 16;

	//move up
	if (currentSwitchValue == up && oldSwitchValue == noInput){
		menuChoiceIndex++;
		//dont run off the edge of the array
		if (menuChoiceIndex > sizeof(rates)/sizeof(rates[0]) - 1){
			menuChoiceIndex = sizeof(rates)/sizeof(rates[0]) - 1;
		}
		drawMenu(menuChoiceIndex);
	}
	//move down
	if (currentSwitchValue == down && oldSwitchValue == noInput){
		menuChoiceIndex--;
		//dont run under the edge of the array
		if (menuChoiceIndex < 0){
			menuChoiceIndex = 0;
		}
		drawMenu(menuChoiceIndex);
	}
	//Press button in to start capture
	if (currentSwitchValue == press && oldSwitchValue == noInput){
		captureInit();
		capture(rates[menuChoiceIndex], data);
	}
	//update rising edge detector
	oldSwitchValue = currentSwitchValue;
}

/**
 * Start capture of data values at a fixed rate.
 * This routine may return before the data
 * capture is complete.
 *
 * @param[in] rate Sample rate (1 to 500 ms)
 * @param[out] data Buffer for data
 *
 */
void capture(int rate, uint8_t array[DATA_POINTS]){
	timerChannel::configure(rate*ms, PitChannelIrq_Enable);
	timerChannel::enableNvicInterrupts(true, NvicPriority_Normal);
}

/**
 * Checks if capture is complete
 *
 * @return true => capture complete
 * @return false => capture busy
 */
bool isCaptureComplete(){
	//Return true if data counter reached the size of the array
	return (dataCounter == DATA_POINTS - 1);
}

/**
 * Pit Call back Function
 * Enables the ADC Interrupt to begin data capture
 */
void PitCallback(void){
	adc::enableNvicInterrupts(true, NvicPriority_Normal);
	potchannel::startConversion(AdcInterrupt_enable);
}

/**
 * ADC Call back function
 */
void adcCallback(uint32_t value, int channel){
	//Display sampling animation
	drawSampling();

	//values from the potentiometer are between 2 and 20
	//Multiplied by 5 because we need a value between 0 and 110
	//Value is directly plotted to LCD pixel position
	data[dataCounter] = value*5;
	//Limit the data value to be diplayable on the LCD screen
	if (data[dataCounter] > MAX_DATA_VALUE){
		data[dataCounter] = MAX_DATA_VALUE;
	}
	//Increment data capture counter
	dataCounter++;
	//Disable adc interrupts after one capture.
	adc::enableNvicInterrupts(false, NvicPriority_Normal);
	//check if complete to disable the PIT
	if (isCaptureComplete()){
		timerChannel::enableNvicInterrupts(false, NvicPriority_Normal);
	}
}

/**
 * Initialise the ADC and PIT to capture data
 */
void  captureInit(){
	//Initialise the ADC
	adc::configure(AdcResolution_12bit_se, AdcClockSource_Bus, AdcClockDivider_2 );
	adc::calibrate();
	adc::setCallback(adcCallback);
	potchannel::setInput();
	potchannel::startConversion(AdcInterrupt_enable);

	//Initialise the Simulated VDD and Ground Pins
	vdd::setOut();
	vdd::write(1);
	ground::setOutput();
	ground::write(0);

	//Initialise the PIT
	timer::configure(PitDebugMode_Stop);
	timerChannel::setCallback(PitCallback);

	//Check for errors
	checkError();
}

/**
 * Displays the Data after capture is complete
 */
void displayData(){
	//Clear the display
	displayInit();
	//Draw the Grid
	drawGrid();
	//Draw the data points
	drawData(data);
}
