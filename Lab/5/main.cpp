#include "hardware.h"
#include "pit.h"
#include "lcd.h"

#include "screen.h" //display grid and menu
#include "capture.h" //capture values form adc using pit
#include "menu.h"  //joystick menu

// Allow access to USBDM methods without USBDM:: prefix
using namespace USBDM;

int main() {
	captureInit();
	displayInit();

	joystickInit();

	for(;;) {
		readJoystick();
	}
}

