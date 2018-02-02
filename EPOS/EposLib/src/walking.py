/*
 * walking.py
 *
 * Desc: Should run a state machine on the legs of the rover
 *
 *  Created on: Feb 1, 2018
 */

import Definitions.h
import EposDriveTrain

#time to wait in ms
sleepTime = 3000 

#Encoder units per rotation
MODVALUE = 175619  # ALL HAIL THE MAGIC NUMBER old one:#176128 
pi = 3.14159265 
START_OFFSET_ANGLE = pi/2 

state = 1 
legAngles [6] 


legAirSpeed = 7.0*350 
landingAngle = 0.349 
legGroundSpeed = legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle))*0.7 

FRONTLEFT   = 1 
MIDDLELEFT  = 2 
BACKLEFT    = 3 
FRONTRIGHT  = 4 
MIDDLERIGHT = 5   
BACKRIGHT   = 6 
accel = 6000 
deccel = 6000 



  # converts curPos (absolute maxon coordinates) and goalAngle (radians) into goalPos (maxon coordinates)
  # goalAngle for all legs is defined to be zero if pointed down and pi/2 if pointing toward back of rover
def getGoalPos(legID,curPos, goalAngle, rotClockwise):
    # get whether the leg is one of the legs on the right side of the rover
    isRightSideLeg = (legID == FRONTRIGHT) or (legID == MIDDLERIGHT) or (legID == BACKRIGHT))
      
    # goalAngleMaxonCoord is the goal angle in maxon coordinates where 0 is pointing down and MODVALUE/4 is pointing
    # toward the back of the rover
    goalAngleMaxonCoord = int(MODVALUE * goalAngle / (2*pi)) 
    curAngleMaxonCoord 
    
    # adjust curPos to account for the offset angle from zero that all motors started at
    # its different for right and left legs because of opposite orientation of the right side motors
    # and the left side motors. Left side motors rotate clockwise toward the front when the maxon coordinates 
    # are increasing
    originalCurPos = curPos 
    
    if (!isRightSideLeg):
        curPos = curPos + (int)(START_OFFSET_ANGLE * MODVALUE / (2 * pi))
    else:
        curPos = curPos - (int)(START_OFFSET_ANGLE * MODVALUE / (2 * pi))
    
    # get the current angle of the leg in terms of maxon coordinates modded to between 0 and MODVALUE
    # this depends on whether the 
    if ((curPos >= 0) and (!isRightSideLeg)):
        curAngleMaxonCoord = (curPos % MODVALUE) 
    else if ((curPos < 0) and (!isRightSideLeg)):
        curAngleMaxonCoord = MODVALUE + (curPos % MODVALUE) 
    else if ((curPos >= 0) and (isRightSideLeg)):
    	curAngleMaxonCoord =  -1 * (curPos % MODVALUE) + MODVALUE 
    else if ((curPos < 0) and (isRightSideLeg)):
    	curAngleMaxonCoord = -1 * (curPos % MODVALUE) 
    
    diff = goalAngleMaxonCoord - curAngleMaxonCoord 
    
    #cout << "legID: " << legID << " originalCurPos: " << originalCurPos << " curPos: " << curPos << " curAngleMaxon: " << curAngleMaxonCoord << " goalMaxon: " << goalAngleMaxonCoord << endl 
    
    delta 
    # Calculate delta (maxon coordinates) that we want to add to the current position
    # Note that if a leg is told to go clockwise to its current position, it 
    if (rotClockwise):
        if (diff > 0):
            delta = diff 
        else:
            delta = diff + MODVALUE
    else:
        if (diff > 0):
            delta = diff - MODVALUE
        else:
            delta = diff


    # if legID is one of the right legs then they are physically oriented such
    # that clockwise motion is negative. Therefore must multiply delta by a factor of -1
    if (isRightSideLeg)
      delta = -1 * delta
    
    return (originalCurPos + delta) 
  

  # moves all three right legs (FRONTRIGHT, BACKRIGHT, MIDDLELEFT)
  # takes in absolute position
def moveLegs(goalAngleRight, velRight, goalAngleLeft, velLeft, driveTrain, rotClockwise):
    # set legs to specified speed
    driveTrain.setPositionProfile(FRONTRIGHT, velRight, accel,deccel) 
    driveTrain.setPositionProfile(MIDDLELEFT, velRight, accel,deccel) 
    driveTrain.setPositionProfile(BACKRIGHT, velRight, accel,deccel) 
    driveTrain.setPositionProfile(FRONTLEFT, velLeft, accel,deccel) 
    driveTrain.setPositionProfile(MIDDLERIGHT, velLeft, accel,deccel) 
    driveTrain.setPositionProfile(BACKLEFT, velLeft, accel,deccel) 

    # Need to get current positions in order to calculate goal position in maxon coordinates
    frontRightCurPos = driveTrain.getPosition(FRONTRIGHT) 
    middleLeftCurPos = driveTrain.getPosition(MIDDLELEFT) 
    backRightCurPos = driveTrain.getPosition(BACKRIGHT) 
    frontLeftCurPos = driveTrain.getPosition(FRONTLEFT) 
    middleRightCurPos = driveTrain.getPosition(MIDDLERIGHT) 
    backLeftCurPos = driveTrain.getPosition(BACKLEFT) 

    # convert to goal position in maxon coordinates using current position (maxon coordinates)
    # and the goalAngle (radians)
    frontRightGoalPos = getGoalPos(FRONTRIGHT, frontRightCurPos, goalAngleRight, rotClockwise) 
    middleLeftGoalPos = getGoalPos(MIDDLELEFT, middleLeftCurPos, goalAngleRight, rotClockwise) 
    backRightGoalPos = getGoalPos(BACKRIGHT, backRightCurPos, goalAngleRight, rotClockwise) 
    frontLeftGoalPos = getGoalPos(FRONTLEFT, frontLeftCurPos, goalAngleLeft, rotClockwise) 
    middleRightGoalPos = getGoalPos(MIDDLERIGHT, middleRightCurPos, goalAngleLeft, rotClockwise) 
    backLeftGoalPos = getGoalPos(BACKLEFT, backLeftCurPos, goalAngleLeft, rotClockwise) 

    # set the position
    driveTrain.setPosition(FRONTRIGHT, frontRightGoalPos, true) 
    driveTrain.setPosition(MIDDLELEFT, middleLeftGoalPos, true) 
    driveTrain.setPosition(BACKRIGHT, backRightGoalPos, true) 
    driveTrain.setPosition(FRONTLEFT, frontLeftGoalPos, true) 
    driveTrain.setPosition(MIDDLERIGHT, middleRightGoalPos, true) 
    driveTrain.setPosition(BACKLEFT, backLeftGoalPos, true) 
  

	# check that all motors have reached their goal position
def allAreAtTargets(EposDriveTrain driveTrain):
  	return 
    (driveTrain.isAtTarget(FRONTRIGHT) and
    driveTrain.isAtTarget(MIDDLELEFT) and
    driveTrain.isAtTarget(BACKRIGHT) and 
    driveTrain.isAtTarget(FRONTLEFT) and
    driveTrain.isAtTarget(MIDDLERIGHT) and
    driveTrain.isAtTarget(BACKLEFT)) 
  }


#Declare new drive train object. The constructor accepts a USB port name as a string
#USB0 is typically the device that the motor controller connects to
driveTrain("USB0") 

print("Initializing...")

#Initialize drive train object
#Note: this object must be initialized by calling init() before trying to move any motors!
driveTrain.init() 

#Enable all nodes, and clear any faults
#Note: before a motor can be moved, it must be enabled!
driveTrain.enableAll() 
driveTrain.clearAllFaults() 

for i in range(1,7):
#Set the i-th node to appropriate mode
driveTrain.setMode(i, PROFILE_POSITION_MODE) 

# flag for making sure state is only called once
moveCommandFlag = true 
  
while(true): 
    # *** STATE MACHINE ***

    # state 1 setups rover depending on initial configuration
    # this could also work with two states, but a setup state might be needed in the future
    if (state == 1):
        # stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
        if (moveCommandFlag):
            moveLegs(landingAngle, legAirSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true) 
        
        if (moveCommandFlag):
            moveCommandFlag = false 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = true 
    }
    # move right feet through air and move left feet on ground
    else if (state == 2):
        if (moveCommandFlag):
            moveLegs(2*pi - landingAngle, legAirSpeed, landingAngle, legGroundSpeed, driveTrain, true) 
        
        if (moveCommandFlag):
            moveCommandFlag = false 
                
        if (allAreAtTargets(driveTrain)):
            state = 3 
            moveCommandFlag = true 

    # move left feet through air and move left feet on ground
    else if (state == 3):
        if (moveCommandFlag):
            moveLegs(landingAngle, legGroundSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, true) 
        
        if (moveCommandFlag):
            moveCommandFlag = false 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = true 
    # #usleep(1000) 

    return 0 