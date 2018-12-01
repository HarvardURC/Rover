# -*- coding: utf-8 -*-
"""
Created on Sat Nov 10 15:09:07 2018

@author: Jenny Gu
"""

import DriveTrain

speed = 1200 
roverAngle; # absolute angle of the rover

driveTrain = DriveTrain.EposDriveTrain()
driveTrain.init()
driveTrain.enableAll()
driveTrain.clearAllFaults()
VELOCITY_POSITION_MODE = 3
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
        while (roverAngle > degree):
            turnLeft90(speed)
    else:
        while (roverAngle < degree):
            turnRight90(speed)

def turnRight45(speed):
    # 45 degree turn
    move(speed, 0)
    
def turnRight90(speed):
    # 90 degree turn
    move(speed, -speed)
    
def turnLeft45(speed):
    # 45 degree turn
    move(0, speed)
    
def turnLeft90(speed):
    # 90 degree turn
    move(-speed, speed)
    
def goStraight(speed):
    move(speed, speed)

def goBackwards(speed):
    move(-speed, speed)

while True:
    move(speed, -speed)
