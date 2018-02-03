import ctypes
import constants
import DriveTrain
import helperFunctions as hF

# GLOBAL VARIABLES
state = 0
doMovement = 'FORWARD'

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

#Set the i-th node to appropriate mode
for i in range(6):
    driveTrain.setMode(i + 1, PROFILE_POSITION_MODE)
# ----------------------------------


# STANDUP ROVER
m = hF.getMoveCommandInfo('STANDUP', 1)
moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)



# flag for making sure state is only called once
moveCommandFlag = True

# --------- STATE MACHINE ------------
while True:
    
    # state 0 setups rover to new doMovement command depending on current configuration
    if state == 0:
        # stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
        if moveCommandFlag:
            curPos = [driveTrain.getPosition(legID) for legID in range(1,7)]
            m = hF.getSetupInfo(doMovement, curPos)
            moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)

        if moveCommandFlag:
            moveCommandFlag = False
        
        if driveTrain.allAreAtTargets():
            state = 1
            moveCommandFlag = True

    # move right feet through air and move left feet on ground
    elif state == 1:
        if moveCommandFlag:
            m = hF.getMoveCommandInfo(doMovement, 1)
            moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)
        
        if moveCommandFlag:
            moveCommandFlag = False
               
        if driveTrain.allAreAtTargets():
            state = 2
            moveCommandFlag = True
        

    # move left feet through air and move left feet on ground
    elif state == 2:
        if moveCommandFlag:
            m = hF.getMoveCommandInfo(doMovement, 2)
            moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)

        if moveCommandFlag:
            moveCommandFlag = False
          
        if driveTrain.allAreAtTargets():
            state = 1
            moveCommandFlag = True

