# Simple demo of of the PCA9685 PWM servo/LED controller library.
# This will move channel 0 from min to max position repeatedly.
# Author: Tony DiCola
# License: Public Domain
import armHelpers as aH
from __future__ import division
import time
import math

# Import the PCA9685 module.
import Adafruit_PCA9685



# Uncomment to enable debug output.
#import logging
#logging.basicConfig(level=logging.DEBUG)

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

# Alternatively specify a different address and/or bus:
#pwm = Adafruit_PCA9685.PCA9685(address=0x41, busnum=2)

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

# GET PINS
'''
L1PIN = int(input("give L1 pin num"))
L2PIN = int(input("give L2 pin num"))
WRISTPANPIN = int(input("give WRISTPAN pin num"))
WRISTTILTPIN = int(input("give WRISTPAN pin num"))
CONTINUOUSPIN = int(input("give CONTINUOUS pin num"))
CLAWPIN = int(input("give CLAW pin num"))
'''
(L1PIN, L2PIN, CONTINUOUSPIN, WRISTPANPIN, WRISTTILTPIN, CLAWPIN) = (0,1,2,3,4,5)


COMMANDS = {
    "wristTilt" : None,
    "wristPan" : None,
    "claw" : None,
    "continuous" : None,
    "l1Theta" : None,
    "l2Theta" : None
}

def getCommands():
    #TODO
    return

while True:
    # GET SERVO COMMANDS
    getCommands()

    # MOVE SERVOS
    if COMMANDS["wristTilt"]:
        pwm.set_pwm(WRISTTILTPIN, 0, COMMANDS["wristTilt"])
    if COMMANDS["wristPan"]:
        pwm.set_pwm(WRISTPANPIN, 0, COMMANDS["wristTilt"])
    if COMMANDS["claw"]:
        print "move claw"
    if COMMANDS["l1Theta"]:
        l1DesPos, l2DesPos = getActuatorPosFromThetas(COMMANDS["l1Theta"], COMMANDS["l1Theta"])
        pwm.set_pwm(L1PIN, 0, l1DesPos)
    if COMMANDS["l2Theta"]:
        l1DesPos, l2DesPos = getActuatorPosFromThetas(COMMANDS["l2Theta"], COMMANDS["l2Theta"])
        pwm.set_pwm(L2PIN, 0, l2DesPos)
    if COMMANDS["continuous"]:
        pwm.set_pwm(L2PIN, 0, l2DesPos)
    
    
    

    pwm.set_pwm(L2PIN, 0, l2DesPos)

    time.sleep(1.0)
