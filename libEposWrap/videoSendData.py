mport time
import pygame
import math
import actuatorHelpers as aH
from pygame.locals import *

LOOPDELAY = .1

wristPan = {
"maxDegree": 87.0,
"minDegree": -93.0,
"homeDegree": 0.0,
"maxpos": 600.0,
"homepos": 380.0,
"minpos": 130.0,
"constdownpos": 132}

wristTilt = {
"maxDegree": 111.0,
"minDegree": -45.0,
"homeDegree": 0.0,
"maxpos": 600.0,
"homepos": 309.0,
"minpos": 190.0}

# TODO: set these to their actual values
camera1Pan = {
"maxDegree": 111.0,
"minDegree": -45.0,
"homeDegree": 0.0,
"maxpos": 600.0,
"homepos": 300.0,
"minpos": 140.0}

camera1Tilt = {
"maxDegree": 111.0,
"minDegree": -45.0,
"homeDegree": 0.0,
"maxpos": 400.0,
"homepos": 300.0,
"minpos": 250.0}


# continuous servo 
continuousModes = {
    "clockwise": 300,
    "counterclockwise": 100,
    "stop": 150
}

Actuator1 = {
"maxlen": 13.6,
"minlen": 10.8,
"maxpos": 365,
"minpos": 275}

Actuator2 = {
"maxlen": 9.75,
"minlen": 6.125,
"maxpos": 475,
"minpos": 250}

COMMANDS = {
    "wristTilt" : wristTilt["homepos"],
    "wristPan" : wristPan["homepos"],
    "claw" : None,
    "continuous" : 150,
    "l1Theta" : 332,
    "l2Theta" : 415,
    "camera1Pan": camera1Pan["homepos"],
    "camera1Tilt": camera1Tilt["homepos"]
}

walkingDirection = "w"
walkingSpeed = 4000
MAXWALKINGSPEED = 5000
MINWALKINGSPEED = 500


theta1 = math.radians(45)
theta2 = math.radians(-45)
(x,z) = aH.getXZFromAngles(theta1,theta2)

IKMode = False
alwaysDownMode = False

def convertDegreeToPos(servo, newDegree):
    #if newDegree > servo["maxDegree"] or newDegree < servo["minDegree"]:
    #    raise Exception("Cannot move servo to that value!")
    
    posOverDegreeRatio = (servo["maxpos"] - servo["minpos"])/float(servo["maxDegree"] - servo["minDegree"])

    posDeltaFromMin = posOverDegreeRatio * float(newDegree - servo["minDegree"])

    return posDeltaFromMin + servo["minpos"]


# checks that we can move servo within limits    
def moveIfSafe(servo, oldPos, newPos):
    if newPos > servo["maxpos"] or newPos < servo["minpos"]:
        return oldPos
    else:
        return newPos
        

'''
Gets joystick data and prints it
'''
pygame.init()
x = pygame.joystick.get_count()
screen = pygame.display.set_mode((400, 300))

# if game controller is available, use game controller
if x:
    print "Using Gamepad for control"

    j = pygame.joystick.Joystick(0)
    j.init()
    print ('Initialized Joystick : %s' % j.get_name())

    which_toggle = ["LeftToggle_horizontal", "LeftToggle_vertical", "RightToggle_horizontal", "RightToggle_vertical",]
    which_button = ["button_X", "button_A", "button_B", "button_Y", "button_LB", "button_RB", "button_LT", "button_RT", "button_back", "button_start", "button_toggleleft", "button_toggleright"]
    try:
        while True:
            pygame.event.pump()

            # read input from the two joysticks  
                 
            for i in range(0, j.get_numaxes()):
                # make sure toggle is past a certain cutoff angle
                if abs(j.get_axis(i)) >= 0.07:
                    #print which_toggle[i], j.get_axis(i)
                        
                    if IKMode:
                        if which_toggle[i] == "RightToggle_vertical":
                            z += -j.get_axis(i) * .3
                        elif which_toggle[i] == "RightToggle_horizontal":
                            x += j.get_axis(i) * .3 

                        (theta1, theta2) = aH.getIKAnglesFromXZ(x,z)
                    else:
                        if which_toggle[i] == "RightToggle_vertical":
                            theta1 += -j.get_axis(i) * math.radians(2)
                        elif which_toggle[i] == "RightToggle_horizontal":
                            theta2 += j.get_axis(i) * math.radians(2)
                        
                        (x,z) = aH.getXZFromAngles(theta1,theta2)
                        
                            
                    (pos1, pos2) = aH.getActuatorPosFromThetas(theta1, theta2)
                    #print "GOALPOSs of actuators", pos1, pos2
                    COMMANDS["l1Theta"] = pos1
                    COMMANDS["l2Theta"] = pos2


                    # wrist
                    if which_toggle[i] == "LeftToggle_vertical":
                        COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], COMMANDS["wristTilt"] - j.get_axis(i) * 10)
                    elif which_toggle[i] == "LeftToggle_horizontal":
                        COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], COMMANDS["wristPan"] + j.get_axis(i) * 10)
            
            COMMANDS["claw"] = 0
            COMMANDS["continuous"] = continuousModes["stop"]
            for i in range(0, j.get_numbuttons()):
                if j.get_button(i) != 0:
                    #print (which_button[i], "button Joystick Num:", i)

                    if which_button[i] == "button_Y":
                        walkingDirection = "w"
                    elif which_button[i] == "button_A":
                        walkingDirection = "s"
                    elif which_button[i] == "button_X":
                        walkingDirection = "a"
                    elif which_button[i] == "button_B":
                        walkingDirection = "d"

                    if which_button[i] == "button_RT":
                        COMMANDS["claw"] = 1
                    elif which_button[i] == "button_LT":
                        COMMANDS["claw"] = 2
                        

                    # continuous
                    if which_button[i] == "button_LB":
                        COMMANDS["continuous"] = continuousModes["counterclockwise"]
                    elif which_button[i] == "button_RB":
                        COMMANDS["continuous"] = continuousModes["clockwise"]

                    if which_button[i] == "button_back":
                        IKMode = not IKMode

            sendString = "" 
            sendString += str(int(COMMANDS["wristTilt"])) + " "
            sendString += str(int(COMMANDS["wristPan"])) + " "
            sendString += str(int(COMMANDS["l1Theta"])) + " "
            sendString += str(int(COMMANDS["l2Theta"])) + " "
            sendString += str(int(COMMANDS["continuous"])) + " "
            sendString += str(int(COMMANDS["claw"])) + " "
            sendString += "-" + str(walkingDirection) + " " + str(walkingSpeed) + '-'

            print sendString
            f = open('controlValues.txt','w')
            f.write(sendString)
            f.close()

            time.sleep(LOOPDELAY)
            
            pygame.event.pump()


    except KeyboardInterrupt:
        j.quit()
        file.close()

else :
    print "Switched to keyboard arrow keys for control"
    while True:
        keys=pygame.key.get_pressed()

        # servo commands
        if keys[K_1]:
            walkingDirection = "w"
        elif keys[K_2]:
            walkingDirection = "s"
        elif keys[K_3]:
            walkingDirection = "a"
        elif keys[K_4]:
            walkingDirection = "d"
        elif keys[K_5]:
            if walkingSpeed / 1.02 > MINWALKINGSPEED:
                walkingSpeed /= 1.015
        elif keys[K_6]:
            if walkingSpeed * 1.02 < MAXWALKINGSPEED:
                walkingSpeed *= 1.015

        # camera
        if keys[K_PERIOD]:
            COMMANDS["camera1Pan"] = moveIfSafe(camera1Pan, COMMANDS["camera1Pan"], COMMANDS["camera1Pan"] + 4)
        elif keys[K_COMMA]:
            COMMANDS["camera1Pan"] = moveIfSafe(camera1Pan, COMMANDS["camera1Pan"], COMMANDS["camera1Pan"] - 4)
        elif keys[K_SEMICOLON]:
            COMMANDS["camera1Tilt"] = moveIfSafe(camera1Tilt, COMMANDS["camera1Tilt"], COMMANDS["camera1Tilt"] + 4)
        elif keys[K_SLASH]:
            COMMANDS["camera1Tilt"] = moveIfSafe(camera1Tilt, COMMANDS["camera1Tilt"], COMMANDS["camera1Tilt"] - 4)
            

        # continuous
        if keys[K_z]:
            COMMANDS["continuous"] = continuousModes["counterclockwise"]
        elif keys[K_c]:
            COMMANDS["continuous"] = continuousModes["clockwise"]
        else:
            COMMANDS["continuous"] = continuousModes["stop"]

        # wrist
        if keys[K_w]:
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], COMMANDS["wristTilt"] + 8)
        elif keys[K_s]:
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], COMMANDS["wristTilt"] - 8)
        elif keys[K_a]:
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], COMMANDS["wristPan"] + 8)
        elif keys[K_d]:
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], COMMANDS["wristPan"] - 8)

        # actuators
        theta1save = theta1
        theta2save = theta2
        xsave = x
        zsave = z

        if IKMode:
            if keys[K_UP]:
                z += .3
            elif keys[K_DOWN]:
                z -= .3
            elif keys[K_LEFT]:
                x -= .3 
            elif keys[K_RIGHT]:
                x += .3
            try:
                (theta1, theta2) = aH.getIKAnglesFromXZ(x,z)
            except:
                x = xsave
                z = zsave
                (theta1, theta2) = aH.getIKAnglesFromXZ(x,z)
        else:
            if keys[K_UP]:
                theta1 += math.radians(1)
            elif keys[K_DOWN]:
                theta1 -= math.radians(1)
            elif keys[K_LEFT]:
                theta2 -= math.radians(1)
            elif keys[K_RIGHT]:
                theta2 += math.radians(1)
            
            try:
                (x,z) = aH.getXZFromAngles(theta1,theta2)
            except:
                theta1 = theta1save 
                theta2 = theta2save
                (x,z) = aH.getXZFromAngles(theta1,theta2)
   
        try:
            (pos1, pos2) = aH.getActuatorPosFromThetas(theta1, theta2)
            COMMANDS["l1Theta"] = pos1
            COMMANDS["l2Theta"] = pos2
        except:
            x = xsave
            z = zsave
            theta1 = theta1save
            theta2 = theta2save
        
        

        if keys[K_p]:
            COMMANDS["claw"] = 2
        elif keys[K_o]:
            COMMANDS["claw"] = 1
        else:
            COMMANDS["claw"] = 0
        
        # go to wrist home pos. h goes to home, g goes to new tilt sensors
        if keys[K_h]:
            alwaysDownMode = False
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], wristTilt["homepos"])
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], wristPan["homepos"])
        
        if keys[K_g]:
            alwaysDownMode = True
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], wristTilt["homepos"])
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], wristPan["homepos"])

        if alwaysDownMode:
            newTiltDegrees = math.degrees(theta1 + theta2) + 90 - 10
            newTiltPos = int(convertDegreeToPos(wristTilt, newTiltDegrees))
            #print newTiltPos, newTiltDegrees
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], newTiltPos)
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], wristPan["constdownpos"])
            

        if keys[K_i]:
            IKMode = True
        elif keys[K_u]:
            IKMode = False
                

        sendString = "" 
        sendString += str(int(COMMANDS["wristTilt"])) + " "
        sendString += str(int(COMMANDS["wristPan"])) + " "
        sendString += str(int(COMMANDS["l1Theta"])) + " "
        sendString += str(int(COMMANDS["l2Theta"])) + " "
        sendString += str(int(COMMANDS["continuous"])) + " "
        sendString += str(int(COMMANDS["claw"])) + " "
        sendString += str(int(COMMANDS["camera1Pan"])) + " " + str(int(COMMANDS["camera1Tilt"])) + " "
        sendString += "-" + str(walkingDirection) + " " + str(walkingSpeed) + '-'

        #print sendString
        f = open('controlValues.txt','w')
        f.write(sendString)
        f.close()

        time.sleep(LOOPDELAY)
        
        pygame.event.pump()



