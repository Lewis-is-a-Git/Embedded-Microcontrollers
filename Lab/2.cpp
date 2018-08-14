#include "hardware.h"

using namespace USBDM;

using SRData    = GpioC<4>;
using SRClk    = GpioC<3>;
using HRClk    = GpioA<1>;
/**
 * Initialise the bar Graph system
 */
void initialiseBarGraph(){
	SRData::setOutput(
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

		if (i < level){
			SRData::write(1);
		}
		else {
			SRData::write(0);
		}

		SRClk::high();
		SRClk::low();
	}
	HRClk::high();
	HRClk::low();
}

int main() {

	initialiseBarGraph();

	for(;;) {

		int level = 4; //just cause
		barGraph(level);

		console.write("The level is: ").writeln(level);
	}
	return 0;
}
