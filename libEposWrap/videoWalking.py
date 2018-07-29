import ctypes
import constants
import DriveTrain
import helperFunctions as hF
import serial


ser = serial.Serial('/dev/ttyACM0',9600)
s = [0]

state = 0
doMovement = 'FORWARD'

#Encoder units per rotation
MODVALUE = 175619;

pi = 3.14159265;
START_OFFSET_ANGLE = pi/2.0;

def getGroundSpeed(airSpeed):
    return int(legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle)));

legAirSpeed = int(2.0*600);
landingAngle = 0.349;
legGroundSpeed = getGroundSpeed(legAirSpeed);
PROFILE_POSITION_MODE = 1

FRONTLEFT   = 1;
MIDDLELEFT  = 2;
BACKLEFT    = 3;
FRONTRIGHT  = 4;
MIDDLERIGHT = 5;  
BACKRIGHT   = 6;
accel = 20000;
deccel = 20000;

tolerance = 30000

lastGoalPosArray = [0]*6;
firstMovement = True


def getMoveCommandInfo(curMovement, state):
    global pi, landingAngle, legAirSpeed, legGroundSpeed
    a = 2*pi - landingAngle
    b = landingAngle
    air = legAirSpeed
    ground = legGroundSpeed

    if curMovement == 'STANDUP':
        goClockwises = [True, True, True, True, True, True]
        legAngles = [0, 0, 0, 0, 0, 0]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'FORWARD':
        goClockwises = [True, True, True, True, True, True]
        if state == 1:
            legAngles = [b, a, b, a, b, a]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif state == 2:
            legAngles = [a, b, a, b, a, b]
            legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'BACKWARD':
        goClockwises = [False, False, False, False, False, False]
        if state == 1:
            legAngles = [a, b, a, b, a, b]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif state == 2:
            legAngles = [b, a, b, a, b, a]
            legSpeeds = [air, ground, air, ground, air, ground]

    elif curMovement == 'ROTATECLOCKWISE': 
        goClockwises = [True, True, True, False, False, False]
        if (state == 1):
            legAngles = [b, a, b, b, a, b]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif (state == 2):
            legAngles = [a, b, a, a, b, a]
            legSpeeds = [air, ground, air, ground, air, ground]

    elif curMovement == 'ROTATECOUNTERCLOCKWISE': 
        goClockwises = [False, False, False, True, True, True]
        if (state == 1):
            legAngles = [a, b, a, a, b, a]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif (state == 2):
            legAngles = [b, a, b, b, a, b]
            legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'STOP':
        goClockwises = [False, False, False, True, True, True]
        if (state == 1):
            legAngles = [a, b, a, a, b, a]
            legSpeeds = [0, 0, 0, 0, 0, 0]
        elif (state == 2):
            legAngles = [b, a, b, b, a, b]
            legSpeeds = [0, 0, 0, 0, 0, 0]       
    # setup object
    m = {}
    m["legAngles"] = legAngles
    m["legSpeeds"] = legSpeeds
    m["goClockwises"] = goClockwises

    return m


def getSetupInfo(curMovement, curPos):
    global pi, landingAngle, legAirSpeed, legGroundSpeed
    a = 2*pi - landingAngle
    b = landingAngle
    air = legAirSpeed
    ground = legGroundSpeed

    if curMovement == 'FORWARD':
        goClockwises = [True, True, True, True, True, True]
        legAngles = [a, b, a, b, a, b]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'BACKWARD':
        goClockwises = [False, False, False, False, False, False]
        legAngles = [b, a, b, a, b, a]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'ROTATECLOCKWISE':
        goClockwises = [True, True, True, False, False, False]
        legAngles = [a, b, a, a, b, a]
        legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'ROTATECOUNTERCLOCKWISE':
        goClockwises = [False, False, False, True, True, True]
        legAngles = [b, a, b, b, a, b]
        legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'STOP':
        goClockwises = [False, False, False, True, True, True]
        legAngles = [b, a, b, b, a, b]
        legSpeeds = [0, 0, 0, 0, 0, 0]
    # setup object
    m = {}
    m["legAngles"] = legAngles
    m["legSpeeds"] = legSpeeds
    m["goClockwises"] = goClockwises

    return m

# Move legs function implemented in python
def moveLegs(goalAngles, vels, goClockwises, driveTrain):
    global pi, landingAngle, legAirSpeed, legGroundSpeed, lastGoalPosArray, firstMovement
    
    # set legs to specified position profile
    for i in range(6):
        driveTrain.setPositionProfile(i + 1, vels[i], accel,deccel);

    # Need to get current positions in order to calculate goal position in maxon coordinates.
    # Convert to goal position in maxon coordinates using current position (maxon coordinates)
    # and the goalAngle (radians)
    curPosArray = []
    goalPosArray = []

    # If it's the first time the legs have been moved, get their actual positions
    if firstMovement:
        for i in range(6):
            curPosArray.append(driveTrain.getPosition(i + 1));
            goalPosArray.append(driveTrain.getGoalPos(i + 1, curPosArray[i], goalAngles[i], goClockwises[i]))
            firstMovement = False
    else:
        for i in range(6):
            curPosArray.append(lastGoalPosArray[i]);
            goalPosArray.append(driveTrain.getGoalPos(i + 1, curPosArray[i], goalAngles[i], goClockwises[i]))


        # Store last goal positions
    lastGoalPosArray = goalPosArray;

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
m = getMoveCommandInfo('STANDUP', 1)
moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)



# flag for making sure state is only called once
moveCommandFlag = True

# --------- STATE MACHINE ------------
while True:
    #f = open('controlValues.txt','r')  
    #readStr = f.read()
    #f.close()
    read_serial=ser.readline()
    direction = read_serial.strip('\t\n\r')
    #s[0] = str(int (ser.readline(),16))
    #print('s0' +s[0] )
    print(read_serial)
    print(doMovement)
    #print readStr
    # if len(readStr.split("-")) > 1:
    #     walkingString = readStr.split("-")[1]
    #     walkingCommands = walkingString.split(" ")
    #     legAirSpeed = int(float(walkingCommands[1]))
    #     legGroundSpeed = getGroundSpeed(legAirSpeed)
    #     direction = walkingCommands[0]
    if direction == "w":
            doMovement = 'FORWARD' 
    elif direction == "s":
            doMovement = 'BACKWARD'
    elif direction == "a":
            doMovement = 'ROTATECOUNTERCLOCKWISE'
    elif direction == "d":
            doMovement = 'ROTATECLOCKWISE'
    elif direction == "x":
            doMovement = 'STOP'
    
    # state 0 setups rover to new doMovement command depending on current configuration
    if state == 0:
        # stateCommandCalled is meant so that moveRightLegs and moveLeftLegs are only called once per state
        if moveCommandFlag:
            curPos = [driveTrain.getPosition(legID) for legID in range(1,7)]
            m = getSetupInfo(doMovement, curPos)
            if doMovement != 'STOP':
                moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)

        if moveCommandFlag:
            moveCommandFlag = False
        
        if driveTrain.areAllCloseEnough(tolerance):
            state = 1
            moveCommandFlag = True

    # move right feet through air and move left feet on ground
    elif state == 1:
        if moveCommandFlag:
            m = getMoveCommandInfo(doMovement, 1)
            if doMovement != 'STOP':
                moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)
        
        if moveCommandFlag:
            moveCommandFlag = False
               
        if driveTrain.areAllCloseEnough(tolerance):
            state = 2
            moveCommandFlag = True
        

    # move left feet through air and move left feet on ground
    elif state == 2:
        if moveCommandFlag:
            m = getMoveCommandInfo(doMovement, 2)
            if doMovement != 'STOP':
                moveLegs(m["legAngles"], m["legSpeeds"], m["goClockwises"], driveTrain)

        if moveCommandFlag:
            moveCommandFlag = False
          
        if driveTrain.areAllCloseEnough(tolerance):
            state = 1
            moveCommandFlag = True





