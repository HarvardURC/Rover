/*
 * EposDriveTrain.cpp
 *
 *  Created on: Nov 11, 2017
 *      Author: matthew
 */

#include "EposDriveTrain.h"

EposDriveTrain::EposDriveTrain(string portName) {
	g_portName = portName;
}

EposDriveTrain::~EposDriveTrain() {
	// TODO Auto-generated destructor stub
}

unsigned int EposDriveTrain::init() {
	//Pointers for passing arguments to the OpenDevice command
	char* deviceName = new char[255];
	char* protocolName = new char[255];
	char* interfaceName = new char[255];
	char* portName = new char[255];

	//Copy parameters into pointers
	strcpy(deviceName, g_deviceName.c_str());
	strcpy(protocolName, g_protocolName.c_str());
	strcpy(interfaceName, g_interfaceName.c_str());
	strcpy(portName, g_portName.c_str());

	//Get handle for accessing device (this sets the global variable)
	portHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);

	//If any errors are thrown, log them here
	if (errorCode != 0) {
		logError("VCS_OpenDevice", errorCode);
	}

	cout << "Port handle: " << portHandle << endl;

	//Free up memory!
	delete[] deviceName;
	delete[] protocolName;
	delete[] interfaceName;
	delete[] portName;

	//If any errors were thrown, return them here
	return errorCode;
}

unsigned int EposDriveTrain::setMode(int node, short opMode) {
	//Set the operation mode with the given library function
	VCS_SetOperationMode(portHandle, node, opMode, &errorCode);

	if(errorCode != 0) {
		logError("VCS_SetOperationMode", errorCode);
	}

	return errorCode;
}

unsigned int EposDriveTrain::setVelocity(int node, long velocity) {
	//Set the velocity at the given controller node
	VCS_MoveWithVelocity(portHandle, node, velocity, &errorCode);

	if(errorCode != 0) {
		logError("VCS_MoveWithVelocity", errorCode);
	}

	return errorCode;
}

unsigned int EposDriveTrain::setPosition(int node, long position, bool absolute) {
	//Set the target position at the given node
	//If absolute=1, then it's an absolute movement
	//If absolute=0, then it's a relative movement
	VCS_MoveToPosition(portHandle, node, position, absolute, 1, &errorCode);

	if(errorCode != 0) {
		logError("VCS_MoveToPosition", errorCode);
	}

	return errorCode;
}

bool EposDriveTrain::stopAllMotors() {
	//Send a stop signal to each of the motor nodes

	//If this whole function succeeds, it returns 1. Otherwise it returns 0
	bool status = 1;

	//Loop through each of the nodes and send a stop signal to each of them
	for (int i=1; i<=6; i++) {
		//Try stopping the motor
		if(!VCS_MoveWithVelocity(portHandle, i, 0, &errorCode)) {
			logError("VCS_MoveWithVelocity", i, errorCode);
			//If it failed, then set the status accordingly
			status = 0;
		}
	}

	return status;
}

bool EposDriveTrain::enableNode(int node) {
	bool status = VCS_SetEnableState(portHandle, node, &errorCode);

	if (!status) {
		logError("VCS_SetEnableState", node, errorCode);
	}

	return status;
}

bool EposDriveTrain::clearFault(int node) {
	bool status = VCS_ClearFault(portHandle, node, &errorCode);

	if (!status) {
		logError("VCS_ClearFault", node, errorCode);
	}

	return status;
}
bool EposDriveTrain::enableAll() {
	//If everything succeeds, return 1. Otherwise return 0
	bool status = 1;

	//Set all motor nodes to 'enable'
	for (int i=1; i<=6; i++) {
		if(!VCS_SetEnableState(portHandle, i, &errorCode)) {
			logError("VCS_SetEnableState", i, errorCode);

			//If it failed, set status accordingly
			status = 0;
		}
	}

	return status;
}

bool EposDriveTrain::clearAllFaults() {
	//If everything succeeds, return 1. Otherwise return 0
	bool status = 1;

	//Set all motor nodes to 'enable'
	for (int i=1; i<=6; i++) {
		if(!VCS_ClearFault(portHandle, i, &errorCode)) {
			logError("VCS_ClearFault", i, errorCode);

			//If it failed, set status accordingly
			status = 0;
		}
	}

	return status;
}

int EposDriveTrain::getPosition(int node) {
	int position;

	if(!VCS_GetPositionIs(portHandle, node, &position, &errorCode)) {
		logError("VCS_GetPositionIs", node, errorCode);
	}

	return position;
}

int EposDriveTrain::getVelocity(int node) {
	int velocity;

	if(!VCS_GetVelocityIs(portHandle, node, &velocity, &errorCode)) {
		logError("VCS_GetVelocityIs", node, errorCode);
	}

	return velocity;
}

bool EposDriveTrain::isAtTarget(int node) {
	int targetReached;

	if(!VCS_GetMovementState(portHandle, node, &targetReached, &errorCode)) {
		logError("VCS_GetMovementState", node, errorCode);
	}

	return (bool) targetReached;
}

short EposDriveTrain::getCurrent(int node) {
	short current;

	if(!VCS_GetCurrentIs(portHandle, node, &current, &errorCode)) {
		logError("VCS_GetMovementState", node, errorCode);
	}

	return current;
}

void EposDriveTrain::logError(string functName, unsigned int errorCode) {
	cerr << functName << " failed. Error code: 0x" << std::hex << errorCode << endl;
}

void EposDriveTrain::logError(string functName, int node, unsigned int errorCode) {
	cerr << functName << " on Node " << node << "failed. Error code: 0x" << std::hex << errorCode << endl;
}
