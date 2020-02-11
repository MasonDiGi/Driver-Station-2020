#include "VARS.h"
#include "Functions.h"
#include "Joystick.h"


bool b1State;
bool b2State;
// Start serial comms and set up button signal and
// RGB pins
void setup() {
	Joystick.clearState();
	//Serial3.begin(9600);
	Serial2.begin(9600);
	SerialUSB.begin(9600);
	setupButtons();
	receiveIndex = 0;
	receiverState = RCV_HUNT;
}

void loop() {
	int dataLen = receiveBytes();
	if (dataLen > 0) {
		parseBytes(dataLen);
	} else if (dataLen == 0) {
		//SerialUSB.println("No data available");
	} else {
		SerialUSB.print("Error occured. Error No: ");
		SerialUSB.print(receiveIndex);
		SerialUSB.print("; ");
		SerialUSB.println(dataLen);
	}
	//prettyColors();
	delay(5);

	int dial1 = 0;


	readButtons();
	sendJoyStates();
}
