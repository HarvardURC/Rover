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
servoDegreesPerSec = 200 
SERVODELTA = LOOPDELAY * servoDegreesPerSec

lidarServo = {
"maxdegree": 90,
"mindegree": -90,
"maxpos": 365,
"minpos": 275}

SWEEPANGLE = 50

class LidarData:
    def __init__(self):
        self.sweepDegrees = []
        self.lidarValues = []
        self.numPoints = 0

    # returns the array of degree values for the servo to check
    def createSweepArray(self, sweepAngle, servoDelta):
        length = int(sweepAngle * 2 / servoDelta)

        lower = 0
        upper = float(sweepAngle)
        rightArray = [lower + x*(upper-lower)/length for x in range(length + 1)]

        lower = float(-sweepAngle)
        upper = 0
        leftArray = [lower + x*(upper-lower)/length for x in range(length + 1)]

        return leftArray + rightArray[1:]

    def getData(self):
        # total sweep is this number of degrees left of zero degrees and right of zero degrees
        self.sweepDegrees = self.createSweepArray(SWEEPANGLE, SERVODELTA)
        self.numPoints = len(self.sweepDegrees)

        # initialize lidar values to zero
        self.lidarValues = [0] * self.numPoints

        lidarIndex = 0
        increasing = True

        while True: 
            # go to next position in array
            gotoAngle = self.sweepDegrees[lidarIndex]
            #pwm.set_pwm(pinID, 0, gotoAngle)
            time.sleep(LOOPDELAY)

            # WHEN LIDAR IS CONNECTED, SWITCH TO THIS
            #distMeasured = lidar.getDistance() 
            distMeasured = 4

            self.lidarValues[lidarIndex] = distMeasured
            # print self.sweepDegrees
            # print self.lidarValues

            # either increment lidar index or decrement depending on whether the loop count is even
            if increasing:
                lidarIndex +=1
            else:
                lidarIndex -= 1
            
            if lidarIndex == 10:
                increasing = False
            elif lidarIndex == 0:
                increasing = True
        return [self.sweepDegrees, self.lidarValues]


ld = LidarData()
ld.getData()
