# -*- coding: utf-8 -*-
"""
Created on Sat Nov 10 15:09:07 2018

@author: Elijah Tai
"""
"""
I edited
"""
import DriveTrain
"""
wheelDist = 51 # 51 centimeters
speed = 1200
wheelRad # radius of wheel
roverAngle; # absolute angle of the rover
"""

driveTrain = DriveTrain.EposDriveTrain()
driveTrain.init()
driveTrain.enableAll()
driveTrain.clearAllFaults()

VELOCITY_POSITION_MODE = 3
POSITION_PROFILE_MODE = 1
HOMING_MODE = 6
INTERPOLATED_POSITION_MODE = 7
POSITION_MODE = -1
VELOCITY_MODE = -2
CURRENT_MODE = -3
MASTER_ENCODER_MODE = -5
STEP_DIRECTION_MODE = -6

for i in range(6):
    driveTrain.setMode(i + 1, POSITION_PROFILE_MODE)
    driveTrain.setPositionProfile(i +1, 0, 0)
    driveTrain.moveToPosition(i+1, 0)

while True:
    val = input("What value doe you want?")
    for i in range(6):
        driveTrain.moveToPosition(i + 1, val)


"""
for i in range(6):
    driveTrain.setMode(i + 1, VELOCITY_POSITION_MODE)
    driveTrain.setVelocityProfile(i + 1, 0, 0);
    driveTrain.setVelocity(i+1, 0)

def move(leftSpeed, rightSpeed):
    for i in range(3):
        driveTrain.setVelocity(i+1, leftSpeed)
        driveTrain.setVelocity(i+4, rightSpeed)

def stop():
    for i in range(6):
        driveTrain.setVelocity(i+1, 0)

def getLocation():
    return "TODO"

def moveTo(location):
    return "TODO"

def turn(degree, speed):
    # positive degree - turn right
    # negative degree - turn left
    if roverAngle - degree > 0:
        # turns left until desired angle is reached
        while (roverAngle > degree):
            turnLeft90(speed)
    else:
        # turns right until desired angle is reached
        while (roverAngle < degree):
            turnRight90(speed)

def turnRight45(speed):
    # 45 degree turn
    move(speed + wheelDist * math.pi / (4 * wheelRad), speed)

def turnRight90(speed):
    # 90 degree turn
    move(speed + wheelDist * math.pi / (2 * wheelRad), speed)

def turnLeft45(speed):
    # 45 degree turn
    move(speed, speed + wheelDist * math.pi / (4 * wheelRad))

def turnLeft90(speed):
    # 90 degree turn
    move(speed, speed + wheelDist * math.pi / (2 * wheelRad))

def goStraight(speed):
    move(speed, speed)

def goBackwards(speed):
    move(-speed, -speed)
"""
