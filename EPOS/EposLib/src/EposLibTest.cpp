/*
 * EposLibTest.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: matthew
 */

#include "Definitions.h"
#include "EposDriveTrain.h"
#include <unistd.h>

using namespace std;

unsigned int errorCode;

//node to test
//int node1Id 1;

//time to wait in ms
long sleepTime = 3000;


int main () {
	//Declare new drive train object
	EposDriveTrain driveTrain("USB0");

	cout << "Initializing..." << endl;

	//Initialize drive train object
	driveTrain.init();

	for (int i = 1; i<6; i++) {

	//clear errors and enable a node
	driveTrain.clearFault(i);
	driveTrain.enableNode(i);

	//Clear all errors
	//driveTrain.clearAllFaults();

	//Enable everything
	//driveTrain.enableAll();

	//Try setting a node to velocity mode
	driveTrain.setMode(i, OMD_PROFILE_VELOCITY_MODE);

	//Try moving a motor
	driveTrain.setVelocity(i, 1000);
	}

	usleep(sleepTime * 5000);

	for (int i = 1; i < 6; i++) {
	//Stop the motor
	driveTrain.setVelocity(i, 0);
	}

	//Stop everything
	//driveTrain.stopAllMotors();

	return 0;
}




