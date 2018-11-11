# Simple demo of of the PCA9685 PWM servo/LED controller library.
# This will move channel 0 from min to max position repeatedly.
# Author: Tony DiCola
# License: Public Domain
from __future__ import division
import time
import math
import RPi.GPIO as GPIO
import json


# Import the PCA9685 module.
import Adafruit_PCA9685

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

# Helper function to make setting a servo pulse width simpler.
def set_servo_pulse(channel, pulse):
    pulse_length = 1000000    # 1,000,000 us per second
    pulse_length //= 60       # 60 Hz
    print('{0}us per period'.format(pulse_length))
    pulse_length //= 4096     # 12 bits of resolution
    print('{0}us per bit'.format(pulse_length))
    pulse *= 1000
    pulse //= pulse_length
    pwm.set_pwm(channel, 0, pulse)

# Set frequency to 60hz, good for servos.
pwm.set_pwm_freq(60)

GPIO.setmode(GPIO.BCM)
GPIO.setup(19, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)


# GET PINS
(CONTINUOUSPIN, L1PIN, L2PIN, WRISTPANPIN, WRISTTILTPIN) = (0,1,2,3,4)

COMMANDS = {}


def getCommands(intArray):
    COMMANDS["wristTilt"] = intArray[0]
    COMMANDS["wristPan"] = intArray[1]
    COMMANDS["l1Theta"] = intArray[2]
    COMMANDS["l2Theta"] = intArray[3]
    COMMANDS["continuous"] = intArray[4]
    COMMANDS["claw"] = intArray[5]
    return COMMANDS


while True:
    f = open('controlValues.txt','r')
    readStr = f.read()
    f.close()

    if len(readStr.split(" ")) > 1:
        intArray = [int(x) for x in readStr.split(" ")[:6]]
        #print intArray

        # GET SERVO COMMANDS
        COMMANDS = getCommands(intArray)
        print COMMANDS

        # MOVE SERVOS
        if COMMANDS["wristTilt"]:
            pwm.set_pwm(WRISTTILTPIN, 0, COMMANDS["wristTilt"])
        if COMMANDS["wristPan"]:
            pwm.set_pwm(WRISTPANPIN, 0, COMMANDS["wristPan"])
        if COMMANDS["claw"]:
            if COMMANDS["claw"] == 2:
                GPIO.output(19, False)
                GPIO.output(16, True)
            elif COMMANDS["claw"] == 1:
                GPIO.output(19, True)
                GPIO.output(16, False)
        else:
            GPIO.output(19, False)
            GPIO.output(16, False)

        if COMMANDS["l1Theta"]:
            pwm.set_pwm(L1PIN, 0, COMMANDS["l1Theta"])
        if COMMANDS["l2Theta"]:
            pwm.set_pwm(L2PIN, 0, COMMANDS["l2Theta"])
        if COMMANDS["continuous"]:
            pwm.set_pwm(CONTINUOUSPIN, 0, COMMANDS["continuous"])
    time.sleep(.3)

