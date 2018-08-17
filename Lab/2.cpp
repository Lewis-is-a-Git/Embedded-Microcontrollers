#include "hardware.h"

using namespace USBDM;

using SER    = GpioD<1>;
using SRClk    = GpioD<3>;
using HRClk    = GpioD<2>;
/**
 * Initialise the bar Graph system
 */
void initialiseBarGraph(){
	SER::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
	SRClk::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
	HRClk::setOutput(
			PinDriveStrength_High,
			PinDriveMode_PushPull,
			PinSlewRate_Slow);
}
/**
 * Displays a value on an 8 bit LED bar
 * @param level - the level to display i.e. number of active LED's (0-8)
 */
void barGraph(int level){

	for (int i = 0; i < 8; i++){
		//only the first sevel leds light up
		if (i < level){
			SER::write(1);
		}else {
			SER::write(0);
		}

		SRClk::high();
		SRClk::low();

	}
	HRClk::high();
	HRClk::low();
}

int main() {

	initialiseBarGraph();
	int level = 0;
	for(;;) {

		barGraph(level);
		level++;
		if (level > 7){
			level = 0;
		}
		waitMS(100);

		console.write("The level is: ").writeln(level);
	}
	return 0;
}
