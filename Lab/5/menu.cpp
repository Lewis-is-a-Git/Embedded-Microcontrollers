/*
 * menu.cpp
 *
 *  Created on: 13 Oct 2018
 *      Author: root
 */
#include "menu.h"

#include "hardware.h"

#include "screen.h" //display menu

using namespace USBDM;

using JOYSTICK_X = Adc0Channel<14>;
using JOYSTICK_Y = Adc0Channel<15>;
using JOYSTICK_K = GpioD<6, ActiveLow>;

/**
 * set joystick k as input (from snippet)
 */
void joystickInit(){
	JOYSTICK_K::setInput();
}
/**
 * Reads the joystick ADC values
 * Joystick might be broken
 * only have left, down and right
 * menu is designed around this limitation
 */
void readJoystick(){
	//left is k = 1
	//right is x = 2
	//down is y = 2

	int  x = JOYSTICK_X::readAnalogue();
	int  y = JOYSTICK_Y::readAnalogue();
	bool k = JOYSTICK_K::read();
	console.write("Joystick (X,Y,K) = ").write(x).write(", ").write(y).write(", ").writeln(k?"HIGH":"LOW");

	static int menuChoice = 0;

	if (y < 10){
		console.writeln("open menu");
		if (menuChoice == 0){
			menuChoice++;
		}
		else{
			menuChoice = 0;
			console.writeln("Close Menu");
		}
		displayMenu(menuChoice);
	}
	if (x < 10){
		console.writeln("move right");
		menuChoice++;
		if (menuChoice > 3){
			menuChoice = 3;
		}
		displayMenu(menuChoice);
	}
	if (k == 1){
		console.writeln("move left");
		menuChoice--;
		if (menuChoice < 1){
			menuChoice = 1;
		}
		displayMenu(menuChoice);
	}
}
