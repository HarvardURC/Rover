//============================================================================
// Name        : EposTest.cpp
// Author      : Matthew Giles
// Version     :
// Copyright   : Not copyrighted, but you probably don't want to use this in its current form
// Description : Test program for use with EPOS4 motor controllers
//============================================================================

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include "Definitions.h"
using namespace std;


//Global variables for EPOS controller parameters
string g_deviceName = "EPOS4";
string g_protocolName = "MAXON SERIAL V2";
string g_interfaceName = "USB";
string g_portName = "USB0";

//Nodes to test
int nodeId = 5;
int node2Id = 4;

//How long motor will run for, in ms
const long timeThreshold = 3000;

long motorSpeed = 1500;

void LogError(string functName, unsigned int errorCode) {
	cerr << functName << " failed. Error code: 0x" << std::hex << errorCode << endl;
}

int main() {
	//Copy parameters for EPOS devices into the string pointers
	char* deviceName = new char[255];
	char* protocolName = new char[255];
	char* interfaceName = new char[255];
	char* portName = new char[255];

	strcpy(deviceName, g_deviceName.c_str());
	strcpy(protocolName, g_protocolName.c_str());
	strcpy(interfaceName, g_interfaceName.c_str());
	strcpy(portName, g_portName.c_str());

	//Variable to store errors returned by EPOS functions
	unsigned int errorCode;

	cout << "Opening Device..." << endl;

	//Open port to send/receive commands
	void * deviceHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);

	cout << "Device handle: " << hex << deviceHandle << endl;

	//If the open device command failed, then it will return 0
	if (deviceHandle == 0) {
		LogError("VCS_OpenDevice", errorCode);
	}


	cout << "Clearing errors..." << endl;

	//Clear error from given node
	if (!VCS_ClearFault(deviceHandle, nodeId, &errorCode)) {
		LogError("VCS_ClearFault", errorCode);
	}


	cout << "Enabling device..." << endl;

	//Enable device on given node
	if (!VCS_SetEnableState(deviceHandle, nodeId, &errorCode)) {
		LogError("VCS_SetEnbableState", errorCode);
	}

	cout << "Setting to velocity mode..." << endl;
	//Set controller to velocity mode
	if (!VCS_SetOperationMode(deviceHandle, nodeId, OMD_PROFILE_VELOCITY_MODE, &errorCode)) {
		LogError("VCS_SetOperationMode", errorCode);
	}

	cout << "Moving motor at " << (int)motorSpeed << "speed..." << endl;

	//Try moving the motor
	if (!VCS_MoveWithVelocity(deviceHandle, nodeId, motorSpeed, &errorCode)) {
		LogError("VCS_MoveWithVelocity", errorCode);
	} else {
		//Sleep for some time
		usleep(timeThreshold * 1000);

		//Stop motor
		if (!VCS_MoveWithVelocity(deviceHandle, nodeId, 0, &errorCode)) {
			LogError("VCS_MoveWithVelocity (stop motor) ", errorCode);
		}
	}

	//Clear errors from second node
	cout << "Clearing errors from second node..." << endl;

	//Clear error from given node
	if (!VCS_ClearFault(deviceHandle, node2Id, &errorCode)) {
		LogError("VCS_ClearFault", errorCode);
	}

	cout << "Enabling second device..." << endl;

	//Enable device on given node
	if (!VCS_SetEnableState(deviceHandle, node2Id, &errorCode)) {
		LogError("VCS_SetEnbableState", errorCode);
	}
	cout << "Setting second controller to velocity mode..." << endl;
	//Set controller to velocity mode
	if (!VCS_SetOperationMode(deviceHandle, node2Id, OMD_PROFILE_VELOCITY_MODE, &errorCode)) {
		LogError("VCS_SetOperationMode", errorCode);
	}

	cout << "Moving motor at " << (int)motorSpeed << "speed..." << endl;

	//Try moving the motor
	if (!VCS_MoveWithVelocity(deviceHandle, node2Id, motorSpeed, &errorCode)) {
		LogError("VCS_MoveWithVelocity", errorCode);
	} else {
		//Sleep for some time
		usleep(timeThreshold * 1000);

		//Stop motor
		if (!VCS_MoveWithVelocity(deviceHandle, node2Id, 0, &errorCode)) {
			LogError("VCS_MoveWithVelocity (stop motor) ", errorCode);
		}
	}


	//Close everything
	VCS_CloseAllDevices(&errorCode);

	//Delete everything
	delete []deviceName;
	delete []protocolName;
	delete []interfaceName;
	delete []portName;

	return 0;
}
