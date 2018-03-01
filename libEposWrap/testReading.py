import time

MODVALUE = 175619;

pi = 3.14159265;
START_OFFSET_ANGLE = pi/2.0;

def getGroundSpeed(airSpeed):
    return int(legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle)));

legAirSpeed = int(7.0*600);
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

tolerance = 40000

lastGoalPosArray = [0]*6;
firstMovement = True




while True:
    f = open('controlValues.txt','r')  
    readStr = f.read()
    f.close()
    walkingString = readStr.split("-")[1]
    walkingCommands = walkingString.split(" ")
    legAirSpeed = float(walkingCommands[1])
    legGroundSpeed = getGroundSpeed(legAirSpeed)
    direction = walkingCommands[0]
    print direction, legAirSpeed, legGroundSpeed
    time.sleep(.4)