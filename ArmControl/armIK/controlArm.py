import json
import time
import pygame
import math

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
"homepos": 308.0,
"minpos": 190.0}

# continuous servo 
continuousModes = {
    "clockwise": 500,
    "counterclockwise": 100,
    "stop": 150
}

COMMANDS = {
    "wristTilt" : wristTilt["homepos"],
    "wristPan" : wristPan["homepos"],
    "claw" : None,
    "continuous" : None,
    "l1Theta" : None,
    "l2Theta" : None
}



def convertDegreeToPos(servo, newDegree):
    if newDegree > servo["maxDegree"] or newDegree < servo["minDegree"]:
        raise Exception("Cannot move servo to that value!")
    
    posOverDegreeRatio = (servo["maxpos"] - servo["minpos"])/float(servo["maxDegree"] - servo["minDegree"])

    posDeltaFromMin = posOverDegreeRatio * float(newDegree - servo["minDegree"])

    return posDeltaFromMin + servo["minpos"]

#def convertPosToDegree(servo, newPos):
    




'''
Gets joystick data and prints it
'''
pygame.init()
#joystick.init()
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
                    print (which_toggle[i], j.get_axis(i), i)
                    
                    file.seek(0)
            
            for i in range(0, j.get_numbuttons()):
                if j.get_button(i) != 0:
                    print (which_button[i], "button Joystick Num:", i)

                    # write to controlValues.txt which button I'm pressing
                    file.seek(0)
                    file.write(str(i))

            time.sleep(.1)

    except KeyboardInterrupt:
        j.quit()
        file.close()

else :
    print "Switched to keyboard arrow keys for control"
    while True:
        keydownEvents = pygame.event.get()
        #heldDownKeys = pygame.key.get_pressed()
        for event in keydownEvents:
            if event.type == pygame.KEYDOWN:
                # continuous
                if event.key == pygame.K_z:
                    COMMANDS["continuous"] = continuousModes["counterclockwise"]
                elif event.key == pygame.K_c:
                    COMMANDS["continuous"] = continuousModes["clockwise"]
                else:
                    COMMANDS["continuous"] = continuousModes["stop"]

                # wrist
                if event.key == pygame.K_w:
                    COMMANDS["wristTilt"] = COMMANDS["wristTilt"] + 3
                elif event.key == pygame.K_s:
                    COMMANDS["wristTilt"] = COMMANDS["wristTilt"] - 3
                elif event.key == pygame.K_a:
                    COMMANDS["wristPan"] = COMMANDS["wristPan"] + 3
                elif event.key == pygame.K_d:
                    COMMANDS["wristPan"] = COMMANDS["wristPan"] - 3

                if event.key == pygame.K_o:
                    COMMANDS["claw"] = "OPEN"
                elif event.key == pygame.K_p:
                    COMMANDS["claw"] = "CLOSE"

        with open('data.txt', 'w') as outfile: 
            print(COMMANDS) 
            json.dump(COMMANDS, outfile)
        
        time.sleep(.2)
    

    