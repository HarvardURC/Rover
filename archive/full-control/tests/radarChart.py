"""
 Show how to do a radar sweep.
 
 Sample Python/Pygame Programs
 Simpson College Computer Science
 http://programarcadegames.com/
 http://simpson.edu/computer-science/
 
"""

import time
import random
'''
from lidarControl import Lidar_Lite
import accelerometerControl as acc

import smbus



from __future__ import division

# Import the PCA9685 module.
import Adafruit_PCA9685

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

pinID = int(input("give the servo controller pin id for the servo: "))

# Set frequency to 60hz, good for servos.
pwm.set_pwm_freq(60)


# LIDAR SETUP -----------------
lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1: 
    print "Not Connected"

'''

LOOPDELAY = .1
servoDegreesPerSec = 60
SERVODELTA = LOOPDELAY * servoDegreesPerSec

lidarServo = {
"maxdegree": 90,
"mindegree": -90,
"maxpos": 365,
"minpos": 275}

SWEEPANGLE = 50

# returns the array of degree values for the servo to check
def createSweepArray(sweepAngle, servoDelta):
    length = int(sweepAngle * 2 / servoDelta)

    lower = 0
    upper = float(sweepAngle)
    rightArray = [lower + x*(upper-lower)/length for x in range(length + 1)]

    lower = float(-sweepAngle)
    upper = 0
    leftArray = [lower + x*(upper-lower)/length for x in range(length + 1)]

    return leftArray + rightArray[1:]




# total sweep is this number of degrees left of zero degrees and right of zero degrees
sweepDegrees = createSweepArray(SWEEPANGLE, SERVODELTA)
numPoints = len(sweepDegrees)

# initialize lidar values to zero
lidarValues = [0] * numPoints

lidarIndex = 0
increasing = True

    




# Import a library of functions called 'pygame'
import pygame
import math
 
# Initialize the game engine
pygame.init()
 
# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
 
PI = 3.141592653
 
# Set the height and width of the screen
SCREENWIDTH = 600
size = [SCREENWIDTH, SCREENWIDTH]
screen = pygame.display.set_mode(size)
 
my_clock = pygame.time.Clock()
 
# Loop until the user clicks the close button.
done = False
 

lineAngle = 0
 
# loop of pygame array
while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    # go to next position in array
    gotoAngle = sweepDegrees[lidarIndex]
    #pwm.set_pwm(pinID, 0, gotoAngle)
    time.sleep(LOOPDELAY)

    #distMeasured = lidar.getDistance() 
    distMeasured = random.random()
    lidarValues[lidarIndex] = distMeasured

    # either increment lidar index or decrement depending on whether the loop count is even
    if increasing:
        lidarIndex +=1
    else:
        lidarIndex -= 1
    
    if lidarIndex == numPoints - 1:
        increasing = False
    elif lidarIndex == 0:
        increasing = True

 
    # Set the screen background
    screen.fill(BLACK)
 
    # Dimensions of radar sweep
    box_dimensions = [0, 0, SCREENWIDTH, SCREENWIDTH]
    centerX = SCREENWIDTH/2
  
 
    # Draw the outline of a circle to 'sweep' the line around
    pygame.draw.ellipse(screen, RED, box_dimensions, 2)
 
    # Draw a black box around the circle
    pygame.draw.rect(screen, BLACK, box_dimensions, 2)

    for i in range(numPoints):
        degree = math.radians(sweepDegrees[i])
        val = lidarValues[i]
        x = int(val * centerX * math.sin(degree) + centerX)
        y = int(-val * centerX * math.cos(degree) + centerX)
        pygame.draw.circle(screen, RED, (x,y), 1)
 
    # Calculate the x,y for the end point of our 'sweep' based on
    # the current lineAngle
    x = centerX * math.sin(lineAngle) + centerX
    y = centerX * math.cos(lineAngle) + centerX
 
    # Draw the line from the center at 145, 145 to the calculated
    # end spot
    pygame.draw.line(screen, GREEN, [centerX, centerX], [x, y], 2)
 
    # Increase the angle by 0.03 radians
    lineAngle = lineAngle + .2
 
    # If we have done a full sweep, reset the lineAngle to 0
    if lineAngle > 2 * PI:
        lineAngle = lineAngle - 2 * PI
 
    # Flip the display, wait out the clock tick
    pygame.display.flip()
    my_clock.tick(60)
 
# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()