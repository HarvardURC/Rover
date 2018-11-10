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
 	//Set the velocity at the given controller node
 	VCS_SetVelocityProfile(portHandle, node, accel, decel, &errorCode);

 	if(errorCode != 0) {
  		logError("VCS_SetVelocityProfile", errorCode);

 	}

 	return errorCode;
 }
unsigned int EposDriveTrain::setPositionProfile(int node, long velocity, long accel, long decel) {

 	VCS_SetPositionProfile(portHandle, node, velocity, accel, decel, &errorCode);

  	if(errorCode != 0) {
 		//logError("VCS_SetPositionProfile", errorCode);
 		//cout << "VCS_SetPositionProfile" << errorCode << endl;
 		//logError("VCS_MoveToPosition", errorCode);
  	}

  	return errorCode;
}
 // Set the target position at the given node
// absolute=1 is an absolute movement, absolute=0 is relative
unsigned int EposDriveTrain::setPosition(int node, long position, bool absolute) {
	VCS_MoveToPosition(portHandle, node, position, absolute, 1, &errorCode);
 	targetPos[node] = position;
 	if(errorCode != 0) {
		logError("VCS_MoveToPosition", errorCode);
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

 bool EposDriveTrain::disableNode(int node) {
	bool status = VCS_SetDisableState(portHandle, node, &errorCode);
 	if (!status) {
		logError("VCS_SetDisableState", node, errorCode);
	}
 	return status;
}
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
// Returns 1 if all nodes are successfully enabled, otherwise 0
bool EposDriveTrain::enableAll() {
	//If everything succeeds, return 1. Otherwise return 0
	bool status = 1;
	// Set all motor nodes to 'enable'
	for (int i=1; i<7; i++) {
		if(!VCS_SetEnableState(portHandle, i, &errorCode)) {
			logError("VCS_SetEnableState", i, errorCode);
 			//If it failed, set status accordingly
			status = 0;
		}
	}
 	return status;
}
 // Returns 1 if all nodes are successfully disabled, otherwise 0
bool EposDriveTrain::disableAll() {
	//If everything succeeds, return 1. Otherwise return 0
	bool status = 1;
	// Set all motor nodes to 'disabled'
	for (int i=1; i<=6; i++) {
		if(!VCS_SetDisableState(portHandle, i, &errorCode)) {
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
	// Set all motor nodes to 'enable'
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

   // converts curPos (absolute maxon coordinates) and goalAngle (radians) into goalPos (maxon coordinates)
  // goalAngle for all legs is defined to be zero if pointed down and pi/2 if pointing toward back of rover
int EposDriveTrain::getGoalPos(int legID, int curPos, float goalAngle, bool rotClockwise) {
	// get whether the leg is one of the legs on the right side of the rover
    bool isRightSideLeg = ((legID == FRONTRIGHT) or (legID == MIDDLERIGHT) or (legID == BACKRIGHT));

    // goalAngleMaxonCoord is the goal angle in maxon coordinates where 0 is pointing down and MODVALUE/4 is pointing
    // toward the back of the rover
    int goalAngleMaxonCoord = (int) (MODVALUE * goalAngle / (2*pi));
    int curAngleMaxonCoord;
     // adjust curPos to account for the offset angle from zero that all motors started at
    // its different for right and left legs because of opposite orientation of the right side motors
    // and the left side motors. Left side motors rotate clockwise toward the front when the maxon coordinates
    // are increasing
    int originalCurPos = curPos;
     if (!isRightSideLeg) {
      curPos = curPos + (int)(START_OFFSET_ANGLE * MODVALUE / (2 * pi));
    } else {
      curPos = curPos - (int)(START_OFFSET_ANGLE * MODVALUE / (2 * pi));
    }
     // get the current angle of the leg in terms of maxon coordinates modded to between 0 and MODVALUE
    // this depends on whether the
    if ((curPos >= 0) and (!isRightSideLeg)){
      curAngleMaxonCoord = (curPos % MODVALUE);
    }
    else if ((curPos < 0) and (!isRightSideLeg)){
      curAngleMaxonCoord = MODVALUE + (curPos % MODVALUE);
    }
    else if ((curPos >= 0) and (isRightSideLeg)){
	    curAngleMaxonCoord =  -1 * (curPos % MODVALUE) + MODVALUE;
    }
    else if ((curPos < 0) and (isRightSideLeg)){
	    curAngleMaxonCoord = -1 * (curPos % MODVALUE);
    }
     int diff = goalAngleMaxonCoord - curAngleMaxonCoord;
     //cout << "legID: " << legID << " originalCurPos: " << originalCurPos << " curPos: " << curPos << " curAngleMaxon: " << curAngleMaxonCoord << " goalMaxon: " << goalAngleMaxonCoord << endl;
     int delta;
    // Calculate delta (maxon coordinates) that we want to add to the current position
    // Note that if a leg is told to go clockwise to its current position, it
    if (rotClockwise) {
      if (diff > 0) {
        delta = diff;
      } else {
        delta = diff + MODVALUE;
      }
    } else {
      if (diff > 0) {
        delta = diff - MODVALUE;
      } else {
        delta = diff;
      }
    }
     // if legID is one of the right legs then they are physically oriented such
    // that clockwise motion is negative. Therefore must multiply delta by a factor of -1
    if (isRightSideLeg)
      delta = -1 * delta;
     return (originalCurPos + delta);
}
 void EposDriveTrain::moveLegs(float * goalAngles, int * vels, bool * goClockwises) {
    // set legs to specified position profile
    for (int i = 1; i <= 6; i++){
      this->setPositionProfile(i, vels[i], accel,deccel);
    }
     // Need to get current positions in order to calculate goal position in maxon coordinates
    int curPosArray[] = {0,0,0,0,0,0};
    for (int i = 1; i <= 6; i++){
      curPosArray[i] = this->getPosition(i);
    }
 	int goalPosArray[6];
     // convert to goal position in maxon coordinates using current position (maxon coordinates)
    // and the goalAngle (radians)
	// copy these goal positions into array
	for (int i = 1; i <= 6; i++) {
		goalPosArray[i] = getGoalPos(i, curPosArray[i], goalAngles[i], goClockwises[i]);
	}
     // set the position
    for (int i = 1; i <= 6; i++){
      this->setPosition(i, goalPosArray[i], true);
    }
}
 bool EposDriveTrain::allAreAtTargets() {
    return
    (this->isAtTarget(FRONTRIGHT) and
    this->isAtTarget(MIDDLELEFT) and
    this->isAtTarget(BACKRIGHT) and
    this->isAtTarget(FRONTLEFT) and
    this->isAtTarget(MIDDLERIGHT) and
    this->isAtTarget(BACKLEFT));
}
 bool EposDriveTrain::isCloseEnough(int node, int tolerance) {
	int position = this->getPosition(node);
	long delta = position - targetPos[node];
 	/* Debugging:
	cout << "Node " << node << " Position: " << position;
	cout << " Target: " << targetPos[node];
	cout << " Delta: " << abs(delta) << " Tolerance: " << tolerance << endl;
	*/

	return ( abs(delta) < tolerance);
}
 bool EposDriveTrain::areAllCloseEnough(int tolerance) {
	bool status = true;
 	for(int i=1; i<=6; i++) {
		if (!(this->isCloseEnough(i, tolerance))) {
			status = false;
		}
	}
 	return status;
}
 void EposDriveTrain::logError(string functName, unsigned int errorCode) {
 	cerr << functName << " failed. Error code: 0x" << std::hex << errorCode << endl;
 }

 void EposDriveTrain::logError(string functName, int node, unsigned int errorCode) {
 	cerr << functName << " on Node " << node << " failed. Error code: 0x" << std::hex << errorCode << endl;
 }
