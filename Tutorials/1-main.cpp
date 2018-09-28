#include "hardware.h" //Templates (Gpio) and enums (PinDriveStrength_High)

using namespace USBDM;

using Switch = GpioC<0, ActiveLow>; // GPIO pin on PORTC pin 0. 

using Led    = GpioC<1>; //GPIO pin on PORTC pin 1. 
using LedRed    = GpioC<3>;
using LedBlue    = GpioA<2>;

int main() {
	//enable clock to pin PORTC.1
	//Configure GpioC->PDDR.1 as output (1)
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
	//enable clock to pin PORTC.0
	//Configure GpioC->PDDR.1 as input (0)
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

