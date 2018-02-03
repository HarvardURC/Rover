'''
walking.py

Desc: Should run a state machine on the legs of the rover
 
Created on: Feb 1, 2018
'''

import Definitions.h
import EposDriveTrain
import DriveTrain
driveTrain = DriveTrain.EposDriveTrain()

#time to wait in ms
sleepTime = 3000 

#Encoder units per rotation
MODVALUE = 175619  # ALL HAIL THE MAGIC NUMBER old one:#176128 
pi = 3.14159265 
START_OFFSET_ANGLE = pi/2 

state = 1 

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


#Declare new drive train object. The constructor accepts a USB port name as a string
#USB0 is typically the device that the motor controller connects to
driveTrain() 

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
moveCommandFlag = True 
  
while(True): 
    # *** STATE MACHINE ***

    # state 1 setups rover depending on initial configuration
    # this could also work with two states, but a setup state might be needed in the future
    if (state == 1):
        # stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
        if (moveCommandFlag):
            moveLegs(landingAngle, legAirSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, True) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = True 
    }
    # move right feet through air and move left feet on ground
    else if (state == 2):
        if (moveCommandFlag):
            moveLegs(2*pi - landingAngle, legAirSpeed, landingAngle, legGroundSpeed, driveTrain, True) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
                
        if (allAreAtTargets(driveTrain)):
            state = 3 
            moveCommandFlag = True 

    # move left feet through air and move left feet on ground
    else if (state == 3):
        if (moveCommandFlag):
            moveLegs(landingAngle, legGroundSpeed, 2*pi - landingAngle, legAirSpeed, driveTrain, True) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = True 

