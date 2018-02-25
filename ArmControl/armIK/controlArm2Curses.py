import curses
import json
import time
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

    

def main(win):
    
    win.nodelay(True)
    key=""
    win.clear()                
    win.addstr("Detected key:")
    while 1:          
        try:                 
           key = win.getkey()
           print key  

           doSomething(key)
                
           win.clear()                
           win.addstr("Detected key:")
           win.addstr(str(key)) 
           
           if key == os.linesep:
              break           
        except Exception as e:
           # No input   
           pass        

curses.wrapper(main)

def doSomething(k):
    key = str(k)
    if key == "z":
        COMMANDS["continuous"] = continuousModes["counterclockwise"]
    elif key == "c":
        COMMANDS["continuous"] = continuousModes["clockwise"]
    else:
        COMMANDS["continuous"] = continuousModes["stop"]

    # wrist
    if key == "w":
        COMMANDS["wristTilt"] = COMMANDS["wristTilt"] + 3
    elif key == "s":
        COMMANDS["wristTilt"] = COMMANDS["wristTilt"] - 3
    elif key == "a":
        COMMANDS["wristPan"] = COMMANDS["wristPan"] + 3
    elif key == "d":
        COMMANDS["wristPan"] = COMMANDS["wristPan"] - 3

    if key == "o":
        COMMANDS["claw"] = "OPEN"
    elif key == "p":
        COMMANDS["claw"] = "CLOSE"

    with open('data.txt', 'w') as outfile: 
        json.dump(COMMANDS, outfile)
    
    