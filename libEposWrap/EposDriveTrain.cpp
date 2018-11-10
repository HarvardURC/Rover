/*
 * EposDriveTrain.cpp
 *
 *  Created on: Nov 11, 2017
 *      Author: Matthew Giles
 */

#include "EposDriveTrain.h"

using namespace std;

EposDriveTrain::EposDriveTrain(/*string portName*/) {
	//g_portName = portName;
}

EposDriveTrain::~EposDriveTrain() {
	// TODO: Make destructor. It should close the device opened in init()
}

unsigned int EposDriveTrain::init() {
	// Pointers for passing arguments to the OpenDevice command
	char* deviceName = new char[255];
	char* protocolName = new char[255];
	char* interfaceName = new char[255];
	char* portName = new char[255];

	// Copy parameters into pointers
	strcpy(deviceName, g_deviceName.c_str());
	strcpy(protocolName, g_protocolName.c_str());
	strcpy(interfaceName, g_interfaceName.c_str());
	strcpy(portName, g_portName.c_str());

	// Get handle for accessing device (this sets the port handle global variable)
	portHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);

	// If any errors are thrown, log them here
	if (errorCode != 0) {
		logError("VCS_OpenDevice", errorCode);
	}

	// Print port handle obtained by OpenDevice
	cout << "Port handle: " << portHandle << endl;

	// Free up memory!
	delete[] deviceName;
	delete[] protocolName;
	delete[] interfaceName;
	delete[] portName;

	// If any errors were thrown, return them here
	return errorCode;
}

unsigned int EposDriveTrain::setMode(int node, short opMode) {
	// Set the operation mode with the given library function
	VCS_SetOperationMode(portHandle, node, opMode, &errorCode);

	if(errorCode != 0) {
		logError("VCS_SetOperationMode", errorCode);
	}

	return errorCode;
}

unsigned int EposDriveTrain::setVelocity(int node, long velocity) {
	// Set the velocity at the given controller node
	VCS_MoveWithVelocity(portHandle, node, velocity, &errorCode);

	if(errorCode != 0) {
		logError("VCS_MoveWithVelocity", errorCode);
	}

	return errorCode;
}

unsigned int EposDriveTrain::setVelocityProfile(int node, long accel, long decel) {
 	VCS_SetVelocityProfile(portHandle, node, accel, decel, &errorCode);

 	if(errorCode != 0) {
 		logError("VCS_SetVelocityProfile", errorCode);

 	}

 	return errorCode;
}

unsigned int EposDriveTrain::setPositionProfile(int node, long vel, long accel, long decel) {
 	VCS_SetPositionProfile(portHandle, node, vel, accel, decel, &errorCode);
  
  	if(errorCode != 0) {
 		logError("VCS_SetPositionProfile", errorCode);
  	}
  
  	return errorCode;
}

bool EposDriveTrain::halt(int node){
	bool status = VCS_HaltVelocityMovement(portHandle, node, &errorCode);

	if (!status) {
		logError("VCS_HaltVelocityMovement", node, errorCode);
	}

	return status;

}

// Send a stop signal to each of the motor nodes
// Returns 1 if all nodes are successfully stopped, otherwise 0
bool EposDriveTrain::stopAllMotors() {
	bool status = 1;

	// Loop through each of the nodes and set velocity to 0
	for (int i=1; i<=6; i++) {
		if(!halt(i)) {
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

bool EposDriveTrain::disableNode(int node) {
	bool status = VCS_SetDisableState(portHandle, node, &errorCode);

	if (!status) {
		logError("VCS_SetDisableState", node, errorCode);
	}

	return status;
}

// Returns 1 if all nodes are successfully enabled, otherwise 0
bool EposDriveTrain::enableAll() {
	bool status = 1;

	// Set all motor nodes to 'enable'
	for (int i=1; i<7; i++) {
		if(!VCS_SetEnableState(portHandle, i, &errorCode)) {
			status = 0;
			logError("VCS_SetEnableState", i, errorCode);
		}
	}

	return status;
}

// Returns 1 if all nodes are successfully disabled, otherwise 0
bool EposDriveTrain::disableAll() {
	bool status = 1;

	// Set all motor nodes to 'disabled'
	for (int i=1; i<=6; i++) {
		if(!VCS_SetDisableState(portHandle, i, &errorCode)) {
			status = 0;
			logError("VCS_SetEnableState", i, errorCode);
		}
	}

	return status;
}

// Resets node, then clears faults
// Node must be re-enabled after a fault disables it
bool EposDriveTrain::clearFault(int node) {
	bool resetStatus = VCS_ResetDevice(portHandle, node, &errorCode);
	bool faultStatus = VCS_ClearFault(portHandle, node, &errorCode);

	if(!resetStatus) {
		logError("VCS_ResetDevice", node, errorCode);
	}

	if (!faultStatus) {
		logError("VCS_ClearFault", node, errorCode);
	}

	return resetStatus && faultStatus;
}

// Returns 1 if faults on all nodes are successfully cleared, otherwise 0
bool EposDriveTrain::clearAllFaults() {
	bool status = 1;

	// Set all motor nodes to 'enable'
	for (int i=1; i<=6; i++) {
		if(!VCS_ClearFault(portHandle, i, &errorCode)) {
			status = 0;
			logError("VCS_ClearFault", i, errorCode);

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

short EposDriveTrain::getCurrent(int node) {
	short current;

	if(!VCS_GetCurrentIs(portHandle, node, &current, &errorCode)) {
		logError("VCS_GetMovementState", node, errorCode);
	}

	return current;
}

bool EposDriveTrain::isAtTarget(int node) {
	int targetReached;

	if(!VCS_GetMovementState(portHandle, node, &targetReached, &errorCode)) {
		logError("VCS_GetMovementState", node, errorCode);
	}

	return (bool) targetReached;
}


void EposDriveTrain::logError(string functName, unsigned int errorCode) {
	cerr << functName << " failed. Error code: 0x" << std::hex << errorCode << endl;
}

void EposDriveTrain::logError(string functName, int node, unsigned int errorCode) {
	cerr << functName << " on Node " << node << " failed. Error code: 0x" << std::hex << errorCode << endl;
}
