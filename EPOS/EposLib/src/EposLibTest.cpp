/*
 * EposLibTest.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: Matthew Giles
 */

#include "Definitions.h"
#include "EposDriveTrain.h"
#include <unistd.h>

using namespace std;

//time to wait in ms
long sleepTime = 3000;


int main () {
	//Declare new drive train object. The constructor accepts a USB port name as a string
	//USB0 is typically the device that the motor controller connects to
	EposDriveTrain driveTrain("USB0");

	cout << "Initializing..." << endl;

	//Initialize drive train object
	//Note: this object must be initialized by calling init() before trying to move any motors!
	driveTrain.init();

	//Enable all nodes, and clear any faults
	//Note: before a motor can be moved, it must be enabled!
	driveTrain.enableAll();
	driveTrain.clearAllFaults();

	for (int i = 2; i<=6; i++) {
	//Set the i-th node to velocity mode
	driveTrain.setMode(i, PROFILE_VELOCITY_MODE);

	//Set the velocity of the i-th node
	driveTrain.setVelocity(i, 1000);
	}

	//Wait for a few seconds
	usleep(sleepTime * 1000);

	for (int i = 2; i <= 6; i++) {
		//Stop each motor
		//stopAllMotors() would also work here; replace this for loop with it
		driveTrain.setVelocity(i, 0);
	}

	//Set node 1 to profile position mode
	driveTrain.setMode(1, PROFILE_POSITION_MODE);

	//Move the motor on node 1 back and forth a few times in position mode
	//ABSOLUTE is a macro declared in EposDriveTrain.h
	//For relative motion, use RELATIVE instead
	for (int i=0; i<2; i++) {
		usleep(3000*1000);

		driveTrain.setPosition(1, 0, ABSOLUTE);

		usleep(3000*1000);

		driveTrain.setPosition(1, 25000, ABSOLUTE);
		//Units used in setPosition are encoder units.
		//What angle this actually corresponds to depends on the encoder, gear ratio, etc.
	}

	return 0;
}
