import ctypes
import constants
import DriveTrain
import fullcode_wheel
import helperFunctions as hF

# GLOBAL VARIABLES
doMovement = 'FORWARD'
accel = 20000
deccel = 20000
air = int(2.0*600)

# Move legs function implemented in python
def moveLegs(goalAngles, vels, goClockwises, driveTrain):
    # set legs to specified position profile
    for i in range(6):
        driveTrain.setPositionProfile(i + 1, vels[i], accel,deccel);

    # Need to get current positions in order to calculate goal position in maxon coordinates.
    # Convert to goal position in maxon coordinates using current position (maxon coordinates)
    # and the goalAngle (radians)
    curPosArray = []
    goalPosArray = []
    for i in range(6):
        curPosArray.append(driveTrain.getPosition(i + 1));
        goalPosArray.append(driveTrain.getGoalPos(i + 1, curPosArray[i], goalAngles[i], goClockwises[i]))

    # set the position
    for i in range(6):
        driveTrain.setPosition(i + 1, goalPosArray[i], True);

# test change here
# --------- MOTOR SETUP ------------
#Declare new drive train object. The constructor accepts a USB port name as a string
#USB0 is typically the device that the motor controller connects to
driveTrain = DriveTrain.EposDriveTrain()
print("Initializing...")

#Initialize drive train object
#Note: this object must be initialized by calling init() before trying to move any motors!
driveTrain.init()

#Enable all nodes, and clear any faults
#Note: before a motor can be moved, it must be enabled!
driveTrain.enableAll()
driveTrain.clearAllFaults()
PROFILE_POSITION_MODE = 1
#Set the i-th node to appropriate mode
for i in range(6):
    driveTrain.setMode(i + 1, PROFILE_POSITION_MODE)
# ----------------------------------


# STANDUP ROVER


# --------- STATE MACHINE ------------
while True:
    
    try:
        velocity = 1
        resetGlobalCheck = 0
        driveTrain.goForward(velocity)
        
    except Exception as error:
        print('Caught this error: ' + repr(error))
        if(resetGlobalCheck == 0):
            resetGlobalCheck = 1
            # CHANGE, put reset code here MATTHEW, (call reset)
            # Reset drive train on error
            fullcode_wheel.resetDriveTrain(driveTrain)