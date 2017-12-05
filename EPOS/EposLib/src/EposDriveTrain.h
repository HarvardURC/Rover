/*
 * EposDriveTrain.h
 *
 *  Created on: Nov 11, 2017
 *      Author: matthew
 */

#ifndef EPOSDRIVETRAIN_H_
#define EPOSDRIVETRAIN_H_

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Definitions.h"
using namespace std;

#define RELATIVE 0
#define ABSOLUTE 1

//TODO: Constants for different modes (profile velocity, profile position, etc)

class EposDriveTrain {
	//Global variables for EPOS controller parameters
	string g_deviceName = "EPOS4";
	string g_protocolName = "MAXON SERIAL V2";
	string g_interfaceName = "USB";
	string g_portName = "USB0"; //Default value. May need to change this.

	void* portHandle;

	//Variable to store error codes
	unsigned int errorCode;

public:
	//Default constructor
	EposDriveTrain(string);

	//"Special" constructor for changing everything
	//EposDriveTrain(string, string, string, string);

	virtual ~EposDriveTrain();

	//Initialization - (calls VCS_OpenDevice)
	unsigned int init();

	//Change operation mode for a given node
	unsigned int setMode(int, short);

	//Move a motor at a velocity
	unsigned int setVelocity(int, long);

	//Move a motor to a position
	unsigned int setPosition(int, long, bool);

	//Stop everything
	bool stopAllMotors();

	//Enable one node
	bool enableNode(int);

	//Clear faults from one node
	bool clearFault(int);

	//Enable all motors
	bool enableAll();

	//Clear errors
	bool clearAllFaults();

	//Get position of a given motor
	int getPosition(int);

	//TODO: getVelocity

	//TODO: getMovementState (i.e. if the drive has reached its target)

	//TODO: getCurrent (motor current drawn by a node)


private:
	void logError(string, unsigned int);
	void logError(string, int, unsigned int);
};

#endif /* EPOSDRIVETRAIN_H_ */
