#include "hardware.h"

using namespace USBDM;

using Switch = GpioC<0, ActiveLow>;

using Led    = GpioC<1>;
using LedRed    = GpioC<3>;
using LedBlue    = GpioA<2>;

int main() {

	Led::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
	LedRed::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
	LedBlue::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);

	Switch::setInput(
			PinPull_Up,
			PinAction_None,
			PinFilter_None);


	for(;;) {

		bool switchValue = Switch::read();
		console.write("Switch is ")
		            		 .writeln(switchValue);

		//LED's are active high
		Led::write(switchValue);
		LedRed::write(!switchValue);
		LedBlue::write(switchValue);
	}
	return 0;
}

