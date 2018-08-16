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
void barGraph(int chase){

	for (int i = 0; i < 8; i++){

		if (i == chase){
			SRData::write(1);
		}else {
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
	int chase = 0;
	for(;;) {

		barGraph(chase);
		chase++;
		if (chase > 7){
			chase = 0;
		}
		waitMS(100);

		console.write("The level is: ").writeln(chase);
	}
	return 0;
}
