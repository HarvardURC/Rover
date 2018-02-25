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
#include "Definitions.h"
using namespace std;

//Constants for position mode
#define RELATIVE 0
#define ABSOLUTE 1

//Constants for different modes
//Used in setMode
#define PROFILE_POSITION_MODE 1
#define PROFILE_VELOCITY_MODE 3
#define POSITION_MODE -1
#define VELOCITY_MODE -2

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
	
	const int MODVALUE = 175619; //ALL HAIL THE MAGIC NUMBER

	//Default constructor
	//EposDriveTrain(string);
	EposDriveTrain();
	const double pi = 3.14159265;
	const float START_OFFSET_ANGLE = pi/2;

	int state = 1;
	int legStates[6] = {0,0,0,0,0,0};
	double legAngles [6];

	long targetPos[6];


	const double legAirSpeed = 7.0*350;
	const double landingAngle = 0.349;
	const double legGroundSpeed = legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle))*0.7;

	const int FRONTLEFT   = 1;
	const int MIDDLELEFT  = 2;
	const int BACKLEFT    = 3;
	const int FRONTRIGHT  = 4;
	const int MIDDLERIGHT = 5;  
	const int BACKRIGHT   = 6;
	const int accel = 6000;
	const int deccel = 6000;
	//const int vel = 6000;

	//"Special" constructor for changing everything
	//EposDriveTrain(string, string, string, string);

	virtual ~EposDriveTrain();

	//Initialization - (calls VCS_OpenDevice)
	unsigned int init();

	//Change operation mode for a given node
	unsigned int setMode(int, short);

	//Move a motor at a velocity
	unsigned int setVelocity(int, long);

	unsigned int setVelocityProfile(int, long, long);

	//Move a motor to a position
	unsigned int setPosition(int, long, bool);

	unsigned int setPositionProfile(int, long, long, long);

	bool halt(int);
	//Stop everything
	bool stopAllMotors();

	//Enable one node
	bool enableNode(int);

	bool disableNode(int);

	//Clear faults from one node
	bool clearFault(int);

	//Enable all motors
	bool enableAll();

	//disable all motors
	bool disableAll();

	//Clear errors
	bool clearAllFaults();

	//Get position of a given motor
	int getPosition(int);

	//Get motor velocity
	int getVelocity(int);

	//Check if a motor has reached its target
	bool isAtTarget(int);

	//Get motor current
	short getCurrent(int);

	//Walking algorithm functions
	int getGoalPos(int, int, float, bool);
	void moveLegs(float*, int*, bool*);
	bool allAreAtTargets();

	bool isCloseEnough(int, int);
	bool areAllCloseEnough(int);

	//unsigned char getNumFaults(int);
	//unsigned int getFaultCode(int, unsigned char);


private:
	void logError(string, unsigned int);
	void logError(string, int, unsigned int);
};

#endif /* EPOSDRIVETRAIN_H_ */
