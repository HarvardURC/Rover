/*
 * EposLibTest.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: Matthew Giles & John Alex Keszler
 */




#include <unistd.h>

#include "constants.h"
using namespace std;


int state = 1;
int legStates[6] = {0,0,0,0,0,0};
double legAngles [6];



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
    
    std::cout << "legID: " << legID << " originalCurPos: " << originalCurPos << " curPos: " << curPos << " curAngleMaxon: " << curAngleMaxonCoord << " goalMaxon: " << goalAngleMaxonCoord << std::endl;
    
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
  void moveLegs(float * goalAngles, int * vels, bool * goClockwises, EposDriveTrain driveTrain){
    // set legs to specified position profile
    for (int i = 0; i < 6; i++){
      int legID = i + 1;
      driveTrain.setPositionProfile(legID, vels[i], accel,deccel);
    }

    // Need to get current positions in order to calculate goal position in maxon coordinates
    int curPosArray[6] = {0,0,0,0,0,0};
    for (int i = 0; i < 6; i++){
      int legID = i + 1;
      curPosArray[i] = driveTrain.getPosition(legID);
    }

    // convert to goal position in maxon coordinates using current position (maxon coordinates)
    // and the goalAngle (radians)
    int goalPosArray[6] = {0,0,0,0,0,0};
    for (int i = 0; i < 6; i++){
      int legID = i + 1;
      goalPosArray[i] = getGoalPos(legID, curPosArray[i], goalPosArray[i], goClockwises[i]);
    }

    // set the position
    for (int i = 0; i < 6; i++){
      int legID = i + 1;
      driveTrain.setPosition(legID, goalPosArray[i], true);
    }

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

  std::cout << "Initializing..." << std::endl;

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
    // *** STATE MACHINE ***
    
    // state 1 setups rover depending on initial configuration
    // this could also work with two states, but a setup state might be needed in the future
    if (state == 1){
      // stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
      if (moveCommandFlag)
      {
        // setup arrays for moveLegs command
        float l = 2*pi - landingAngle;
        float r = landingAngle;
        float legAngles[6] = {l, r, l, r, l, r};
        int legSpeeds[6] = {legAirSpeed, legAirSpeed, legAirSpeed, legAirSpeed, legAirSpeed, legAirSpeed};
        bool goClockwises[6] = {true, true, true, true, true, true};
        moveLegs(legAngles, legSpeeds, goClockwises, driveTrain);
        //moveLegs(landingAngle, legAirSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true);
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
        // setup arrays for moveLegs command
        float l = landingAngle;
        float r = 2*pi - landingAngle;
        float legAngles[6] = {l, r, l, r, l, r};
        int legSpeeds[6] = {legGroundSpeed, legAirSpeed, legGroundSpeed, legAirSpeed, legGroundSpeed, legAirSpeed};
        bool goClockwises[6] = {true, true, true, true, true, true};
        moveLegs(legAngles, legSpeeds, goClockwises, driveTrain);
        //moveRightLegs(2*pi - landingAngle, legAirSpeed, landingAngle, legGroundSpeed, driveTrain, true);
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
        // setup arrays for moveLegs command
        float l = 2*pi - landingAngle;
        float r = landingAngle;
        float legAngles[6] = {l, r, l, r, l, r};
        int legSpeeds[6] = {legAirSpeed, legGroundSpeed, legAirSpeed, legGroundSpeed, legAirSpeed, legGroundSpeed};
        bool goClockwises[6] = {true, true, true, true, true, true};
        moveLegs(legAngles, legSpeeds, goClockwises, driveTrain);
        //moveRightLegs(landingAngle, legGroundSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true);
      }
      
      if (moveCommandFlag){
      	moveCommandFlag = false;
      }
        
      if (allAreAtTargets(driveTrain)){
        state = 2;
        moveCommandFlag = true;
      }
    } 
  }

    return 0;
}
