/*
 * EposDriveTrain.h
 *
 *  Created on: Nov 11, 2017
 *      Author: Matthew Giles
 */

#ifndef EPOSDRIVETRAIN_H_
#define EPOSDRIVETRAIN_H_

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// EPOS library definitions file
#include "Definitions.h"

// Constants for different modes, used in setMode
#define PROFILE_POSITION_MODE 1
#define PROFILE_VELOCITY_MODE 3
#define POSITION_MODE -1
#define VELOCITY_MODE -2

// Constants for position mode
#define RELATIVE 0
#define ABSOLUTE 1

using namespace std;

class EposDriveTrain {
	// Global variables for EPOS controller parameters
	string g_deviceName = "EPOS4";
	string g_protocolName = "MAXON SERIAL V2";
	string g_interfaceName = "USB";
	string g_portName = "USB0";

	// Handle for accessing device
	void* portHandle;

	// Variable to store error codes
	unsigned int errorCode;

public:
	// Encoder units per full rotation
	// ALL HAIL THE MAGIC NUMBER
	const int MODVALUE = 175619; 

	// Default constructor
	EposDriveTrain();

	// Destructor
	virtual ~EposDriveTrain();

	// Initialization - (calls VCS_OpenDevice)
	unsigned int init();

	// Change operation mode for a given node
	unsigned int setMode(int, short);

	// Move a motor at a velocity
	unsigned int setVelocity(int, long);
	unsigned int setVelocityProfile(int, long, long);

	// Move a motor to a position
	unsigned int setPosition(int, long, bool);
	unsigned int setPositionProfile(int, long, long, long);

	// Stop motors
	bool halt(int);
	bool stopAllMotors();

	// Enable/Disable motors
	bool enableNode(int);
	bool disableNode(int);
	bool enableAll();
	bool disableAll();

	// Clear faults
	bool clearFault(int);
	bool clearAllFaults();

	// Get position, velocity, or current draw of a node
	int getPosition(int);
	int getVelocity(int);
	short getCurrent(int);

	// Check if a motor has reached its target
	bool isAtTarget(int);

	// TODO: Get number of faults and get fault code
	//unsigned char getNumFaults(int);
	//unsigned int getFaultCode(int, unsigned char);


private:
	void logError(string, unsigned int);
	void logError(string, int, unsigned int);
};

#endif /* EPOSDRIVETRAIN_H_ */
