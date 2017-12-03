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
	for (int i=1; i<6; i++) {
		//Try stopping the motor
		if(!VCS_MoveWithVelocity(portHandle, i, 0, &errorCode)) {
			cerr << "VCS_MoveWithVelocity on Node " << i << "failed. Error code: 0x" << std::hex << errorCode << endl;
			//If it failed, then set the status accordingly
			status = 0;
		}
	}

	return status;
}

bool EposDriveTrain::enableNode(int node) {
	bool status = VCS_SetEnableState(portHandle, node, &errorCode);

	if (!status) {
		cerr << "VCS_SetEnableState on Node " << node << " failed. Error code: 0x" << std::hex << errorCode << endl;
	}

	return status;
}

bool EposDriveTrain::clearFault(int node) {
	bool status = VCS_ClearFault(portHandle, node, &errorCode);

	if (!status) {
		cerr << "VCS_ClearFault on Node " << node << " failed. Error code: 0x" << std::hex << errorCode << endl;
	}

	return status;
}
bool EposDriveTrain::enableAll() {
	//If everything succeeds, return 1. Otherwise return 0
	bool status = 1;

	//Set all motor nodes to 'enable'
	for (int i=1; i<6; i++) {
		if(!VCS_SetEnableState(portHandle, i, &errorCode)) {
			cerr << "VCS_SetEnableState on Node " << i << " failed. Error code: 0x" << std::hex << errorCode << endl;
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
	for (int i=1; i<6; i++) {
		if(!VCS_ClearFault(portHandle, i, &errorCode)) {
			cerr << "VCS_ClearFault on Node " << i << "failed. Error code: 0x" << std::hex << errorCode << endl;

			//If it failed, set status accordingly
			status = 0;
		}
	}

	return status;
}

int EposDriveTrain::getPosition(int node) {
	int position;

	if(!VCS_GetPositionIs(portHandle, node, &position, &errorCode)) {
		cerr << "VCS_GetPositionIs on Node " << node << "failed. Error code: 0x" << std::hex << errorCode << endl;
	}

	return position;
}

void EposDriveTrain::logError(string functName, unsigned int errorCode) {
	cerr << functName << " failed. Error code: 0x" << std::hex << errorCode << endl;
}
