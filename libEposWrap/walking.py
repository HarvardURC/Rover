'''
walking.py

Desc: Should run a state machine on the legs of the rover
 
Created on: Feb 1, 2018
'''

import DriveTrain
import ctypes
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
PROFILE_POSITION_MODE = 1
accel = 6000 
deccel = 6000 


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

a = float(2*pi - landingAngle)
b = float(landingAngle)
air = int(legAirSpeed)
ground = int(legGroundSpeed)

''' 
legAngles = DriveTrain.floatArray()
legSpeeds = DriveTrain.intArray()
goClockwises = DriveTrain.boolArray()
'''

while(True): 
    # *** STATE MACHINE ***
    goClockwises = (ctypes.c_bool * 6)(True, True, True, True, True, True)
    # state 1 setups rover depending on initial configuration
    # this could also work with two states, but a setup state might be needed in the future
    if (state == 1):
        # stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
        if (moveCommandFlag):
            legAngles = (ctypes.c_float * 6)(a, b, a, b, a, b)
            legSpeeds = (ctypes.c_int * 6)(air, air, air, air, air, air)
            driveTrain.moveLegs(legAngles, legSpeeds, goClockwises) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = True 
    
    # move right feet through air and move left feet on ground
    elif (state == 2):
        if (moveCommandFlag):
            legAngles = (ctypes.c_float * 6)(b, a, b, a, b, a)
            legSpeeds = (ctypes.c_int * 6)(ground, air, ground, air, ground, air)
            driveTrain.moveLegs(legAngles, legSpeeds, goClockwises) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
                
        if (allAreAtTargets(driveTrain)):
            state = 3 
            moveCommandFlag = True 

    # move left feet through air and move left feet on ground
    elif (state == 3):
        if (moveCommandFlag):
            legAngles = (ctypes.c_float * 6)(a, b, a, b, a, b)
            legSpeeds = (ctypes.c_int * 6)(air, ground, air, ground, air, ground)
            driveTrain.moveLegs(legAngles, legSpeeds, goClockwises) 
        
        if (moveCommandFlag):
            moveCommandFlag = False 
        
        if (allAreAtTargets(driveTrain)):
            state = 2 
            moveCommandFlag = True 

