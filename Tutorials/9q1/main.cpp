/**
 ============================================================================
 * @brief   Control servo motor using PWM signal
 *
 *  Created on: 11/10/2018
 *      Author: Lewis
 ============================================================================
 */
#include "hardware.h"

#include "servo.h"

using namespace USBDM;

int main() {

	initialiseServo();

	// main loop
	for(;;) {

		// Using high-time
		for (unsigned i=0; i<101; i++) {
			setServoPosition(i);
			waitMS(10);
		}
	}
}
