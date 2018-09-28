/*
 ============================================================================
 * @file    main.cpp
 * @brief   quizgame
 *
 *  Created on: 27/9/2018
 *      Author: Lewis
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

void initialiseGame(){
	Lights::setOutput(PinDriveStrength_Low, PinDriveMode_PushPull, PinSlewRate_Slow);
	Buttons::setInput(PinPull_Up,PinAction_None, PinFilter_Passive);
	Restart::setInput(PinPull_Up,PinAction_None, PinFilter_Passive);
	Buzzer::setOutput(PinDriveStrength_Low, PinDriveMode_PushPull, PinSlewRate_Slow);
}

int main() {
	initialiseGame();

	while(true){
		Buzzer::off();
		Lights::write(0);

		//take snapshot
		int contestantButtons = 0;
		do {
			contestantButtons = Buttons::read();
		}while(contestantButtons == 0); //wait for contestant button press
		//process snapshot
			//no processing needs to be done can directly write button input to light output
		//turn on lights
		Lights::write(snapShot);
		//turn on buzzer
		Buzzer::on();
		waitMS(1000);
		Buzzer::off();

		//wait for restart button pushed
		while(Restart::isInactive()){}

		//Debounce Delay
		waitMS(50);
		while (Restart::isActive()){}

		//Debounce Delay
		waitMS(50;)
	}

	return 0;
}
