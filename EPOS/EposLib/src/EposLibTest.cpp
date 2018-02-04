/*
 * EposLibTest.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: Matthew Giles & John Alex Keszler
 */

#include "Definitions.h"
#include "EposDriveTrain.h"
#include <unistd.h>
using namespace std;

//time to wait in ms
long sleepTime = 3000;

//Encoder units per rotation
int MODVALUE = 175619; // ALL HAIL THE MAGIC NUMBER old one://176128;
const double pi = 3.14159265;
const float START_OFFSET_ANGLE = pi/2;

int state = 1;
int legStates[6] = {0,0,0,0,0,0};
double legAngles [6];


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



  // converts curPos (absolute maxon coordinates) and goalAngle (radians) into goalPos (maxon coordinates)
  // goalAngle for all legs is defined to be zero if pointed down and pi/2 if pointing toward back of rover
  int getGoalPos(int legID, int curPos, float goalAngle, bool rotClockwise){
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

  // moves all three right legs (FRONTRIGHT, BACKRIGHT, MIDDLELEFT)
  // takes in absolute position
  void moveLegs(float goalAngleRight, int velRight, float goalAngleLeft, int velLeft, EposDriveTrain driveTrain, bool rotClockwise){
    // set legs to specified speed
    driveTrain.setPositionProfile(FRONTRIGHT, velRight, accel,deccel);
    driveTrain.setPositionProfile(MIDDLELEFT, velRight, accel,deccel);
    driveTrain.setPositionProfile(BACKRIGHT, velRight, accel,deccel);
    driveTrain.setPositionProfile(FRONTLEFT, velLeft, accel,deccel);
    driveTrain.setPositionProfile(MIDDLERIGHT, velLeft, accel,deccel);
    driveTrain.setPositionProfile(BACKLEFT, velLeft, accel,deccel);

    // Need to get current positions in order to calculate goal position in maxon coordinates
    int frontRightCurPos = driveTrain.getPosition(FRONTRIGHT);
    int middleLeftCurPos = driveTrain.getPosition(MIDDLELEFT);
    int backRightCurPos = driveTrain.getPosition(BACKRIGHT);
    int frontLeftCurPos = driveTrain.getPosition(FRONTLEFT);
    int middleRightCurPos = driveTrain.getPosition(MIDDLERIGHT);
    int backLeftCurPos = driveTrain.getPosition(BACKLEFT);

    // convert to goal position in maxon coordinates using current position (maxon coordinates)
    // and the goalAngle (radians)
    int frontRightGoalPos = getGoalPos(FRONTRIGHT, frontRightCurPos, goalAngleRight, rotClockwise);
    int middleLeftGoalPos = getGoalPos(MIDDLELEFT, middleLeftCurPos, goalAngleRight, rotClockwise);
    int backRightGoalPos = getGoalPos(BACKRIGHT, backRightCurPos, goalAngleRight, rotClockwise);
    int frontLeftGoalPos = getGoalPos(FRONTLEFT, frontLeftCurPos, goalAngleLeft, rotClockwise);
    int middleRightGoalPos = getGoalPos(MIDDLERIGHT, middleRightCurPos, goalAngleLeft, rotClockwise);
    int backLeftGoalPos = getGoalPos(BACKLEFT, backLeftCurPos, goalAngleLeft, rotClockwise);

    // set the position
    driveTrain.setPosition(FRONTRIGHT, frontRightGoalPos, true);
    driveTrain.setPosition(MIDDLELEFT, middleLeftGoalPos, true);
    driveTrain.setPosition(BACKRIGHT, backRightGoalPos, true);
    driveTrain.setPosition(FRONTLEFT, frontLeftGoalPos, true);
    driveTrain.setPosition(MIDDLERIGHT, middleRightGoalPos, true);
    driveTrain.setPosition(BACKLEFT, backLeftGoalPos, true);
  }

	// check that all motors have reached their goal position
  bool allAreAtTargets(EposDriveTrain driveTrain){
  	return 
    (driveTrain.isAtTarget(FRONTRIGHT) and
    driveTrain.isAtTarget(MIDDLELEFT) and
    driveTrain.isAtTarget(BACKRIGHT) and 
    driveTrain.isAtTarget(FRONTLEFT) and
    driveTrain.isAtTarget(MIDDLERIGHT) and
    driveTrain.isAtTarget(BACKLEFT));
  }


int main () {
  //Declare new drive train object. The constructor accepts a USB port name as a string
  //USB0 is typically the device that the motor controller connects to
  EposDriveTrain driveTrain("USB0");

  cout << "Initializing..." << endl;

  //Initialize drive train object
  //Note: this object must be initialized by calling init() before trying to move any motors!
  driveTrain.init();

  //Enable all nodes, and clear any faults
  //Note: before a motor can be moved, it must be enabled!
  driveTrain.enableAll();
  driveTrain.clearAllFaults();

  for (int i = 1; i<=6; i++) {
    //Set the i-th node to appropriate mode
    driveTrain.setMode(i, PROFILE_POSITION_MODE);
  }
  
  // flag for making sure state is only called once
  bool moveCommandFlag = true;
  
  while(true)
  {  
    
  	 // moveRightLegs(landingAngle, legAirSpeed, driveTrain, true);
  	 // sleep(4);
  	 //moveRightLegs(2*pi - landingAngle, legAirSpeed, driveTrain, true);
  	 // moveLeftLegs(pi, legGroundSpeed, driveTrain, true);
  	 // moveLeftLegs(2*pi/3, legAirSpeed, driveTrain, true);
  	 // sleep(6);
  	 //sleep(4);
  	 
    
    // *** STATE MACHINE ***
    
    // state 1 setups rover depending on initial configuration
    // this could also work with two states, but a setup state might be needed in the future
    if (state == 1){
      // stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
      if (moveCommandFlag)
      {
        moveLegs(landingAngle, legAirSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true);
      }
      
      if (moveCommandFlag){
      	moveCommandFlag = false;
      }
      
      if (allAreAtTargets(driveTrain)){
        state = 2;
        moveCommandFlag = true;
      }
    }
    // move right feet through air and move left feet on ground
    else if (state == 2){
      if (moveCommandFlag) {
        moveLegs(2*pi - landingAngle, legAirSpeed, landingAngle, legGroundSpeed, driveTrain, true);
      }
      
      if (moveCommandFlag){
      	moveCommandFlag = false;
      }
             
      if (allAreAtTargets(driveTrain)){
        state = 3;
        moveCommandFlag = true;
      }
          
    } 
    // move left feet through air and move left feet on ground
    else if (state == 3){
      if (moveCommandFlag){
        moveLegs(landingAngle, legGroundSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true);
      }
      
      if (moveCommandFlag){
      	moveCommandFlag = false;
      }
        
      if (allAreAtTargets(driveTrain)){
        state = 2;
        moveCommandFlag = true;
      }
    } 
    // //usleep(1000);
  }

    return 0;
}