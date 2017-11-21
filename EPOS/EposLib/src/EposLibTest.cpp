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
int node1ID = 6;

//time to wait in ms
long sleepTime = 3000;


int main () {
	//Declare new drive train object
	EposDriveTrain driveTrain("USB0");

	cout << "Initializing..." << endl;

	//Initialize drive train object
	driveTrain.init();

	//Clear all errors
	driveTrain.clearAllFaults();

	//Enable everything
	driveTrain.enableAll();

	//Try setting a node to velocity mode
	driveTrain.setMode(node1ID, OMD_PROFILE_VELOCITY_MODE);

	//Try moving a motor
	driveTrain.setVelocity(node1ID, 1000);

	usleep(sleepTime * 1000);

	//Stop the motor
	driveTrain.setVelocity(node1ID, 0);

	//Stop everything
	//driveTrain.stopAllMotors();

	return 0;
}




