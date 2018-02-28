import json
import time
import pygame
import math
import actuatorHelpers as aH
from pygame.locals import *

LOOPDELAY = .2

wristPan = {
"maxDegree": 87.0,
"minDegree": -93.0,
"homeDegree": 0.0,
"maxpos": 600.0,
"homepos": 380.0,
"minpos": 130.0}

wristTilt = {
"maxDegree": 111.0,
"minDegree": -45.0,
"homeDegree": 0.0,
"maxpos": 600.0,
"homepos": 309.0,
"minpos": 190.0}

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
    "l2Theta" : 415
}

doMovement = 1

theta1 = math.radians(45)
theta2 = math.radians(-45)

IKMode = False


def convertDegreeToPos(servo, newDegree):
    if newDegree > servo["maxDegree"] or newDegree < servo["minDegree"]:
        raise Exception("Cannot move servo to that value!")
    
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
                        doMovement = 1
                    elif which_button[i] == "button_A":
                        doMovement = 2
                    elif which_button[i] == "button_X":
                        doMovement = 3
                    elif which_button[i] == "button_B":
                        doMovement = 4

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
            sendString += str(doMovement) + " "
            sendString += str(int(COMMANDS["wristTilt"])) + " "
            sendString += str(int(COMMANDS["wristPan"])) + " "
            sendString += str(int(COMMANDS["l1Theta"])) + " "
            sendString += str(int(COMMANDS["l2Theta"])) + " "
            sendString += str(int(COMMANDS["continuous"])) + " "
            sendString += str(int(COMMANDS["claw"])) + " "

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
            doMovement = 1
        elif keys[K_2]:
            doMovement = 2
        elif keys[K_3]:
            doMovement = 3
        elif keys[K_4]:
            doMovement = 4

        # continuous
        if keys[K_z]:
            COMMANDS["continuous"] = continuousModes["counterclockwise"]
        elif keys[K_c]:
            COMMANDS["continuous"] = continuousModes["clockwise"]
        else:
            COMMANDS["continuous"] = continuousModes["stop"]

        # wrist
        if keys[K_w]:
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], COMMANDS["wristTilt"] + 10)
        elif keys[K_s]:
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], COMMANDS["wristTilt"] - 10)
        elif keys[K_a]:
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], COMMANDS["wristPan"] + 10)
        elif keys[K_d]:
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], COMMANDS["wristPan"] - 10)

        # actuators
        if IKMode:
            if keys[K_UP]:
                z += .3
            elif keys[K_DOWN]:
                z -= .3
            elif keys[K_LEFT]:
                x -= .3 
            elif keys[K_RIGHT]:
                x += .3

            (theta1, theta2) = aH.getIKAnglesFromXZ(x,z)
        else:
            if keys[K_UP]:
                theta1 += math.radians(2)
            elif keys[K_DOWN]:
                theta1 -= math.radians(2)
            elif keys[K_LEFT]:
                theta2 += math.radians(2)
            elif keys[K_RIGHT]:
                theta2 -= math.radians(2)
            
            (x,z) = aH.getXZFromAngles(theta1,theta2)
            
                
        (pos1, pos2) = aH.getActuatorPosFromThetas(theta1, theta2)
        #print "GOALPOSs of actuators", pos1, pos2
        COMMANDS["l1Theta"] = pos1
        COMMANDS["l2Theta"] = pos2

        if keys[K_o]:
            COMMANDS["claw"] = 1
        elif keys[K_p]:
            COMMANDS["claw"] = 2
        else:
            COMMANDS["claw"] = 0
        
        # go to wrist home pos. h goes to home, g goes to new tilt sensors
        if keys[K_h]:
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], wristTilt["homepos"])
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], wristPan["homepos"])
        elif keys[K_g]:
            newTiltDegrees = math.degrees(theta1 + theta2) + 90
            newTiltPos = int(convertDegreeToPos(wristTilt, newTiltDegrees))
            #print newTiltPos, newTiltDegrees
            COMMANDS["wristTilt"] = moveIfSafe(wristTilt, COMMANDS["wristTilt"], newTiltPos)
            COMMANDS["wristPan"] = moveIfSafe(wristPan, COMMANDS["wristPan"], wristPan["homepos"])
            

        if keys[K_i]:
            IKMode = True
        elif keys[K_u]:
            IKMode = False
                

        sendString = "" 
        sendString += str(doMovement) + " "
        sendString += str(int(COMMANDS["wristTilt"])) + " "
        sendString += str(int(COMMANDS["wristPan"])) + " "
        sendString += str(int(COMMANDS["l1Theta"])) + " "
        sendString += str(int(COMMANDS["l2Theta"])) + " "
        sendString += str(int(COMMANDS["continuous"])) + " "
        sendString += str(int(COMMANDS["claw"])) + " "

        print sendString
        f = open('controlValues.txt','w')
        f.write(sendString)
        f.close()

        time.sleep(LOOPDELAY)
        
        pygame.event.pump()
        
    

    