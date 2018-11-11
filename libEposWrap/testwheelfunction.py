import ctypes
import constants
import DriveTrain
import helperFunctions as hF

# GLOBAL VARIABLES
doMovement = 'FORWARD'
accel = 20000
deccel = 20000
air = int(2.0*600)


# test change here
# --------- MOTOR SETUP ------------
#Declare new drive train object. The constructor accepts a USB port name as a string
#USB0 is typically the device that the motor controller connects to
driveTrain = DriveTrain.EposDriveTrain()
print("Initializing...")

#Initialize drive train object
#Note: this object must be initialized by calling init() before trying to move any motors!
driveTrain.init()

#Enable all nodes, and clear any faults
#Note: before a motor can be moved, it must be enabled!
driveTrain.enableAll()
driveTrain.clearAllFaults()
VELOCITY_MODE = -2
#Set the i-th node to appropriate mode
for i in range(6):
    driveTrain.setMode(i + 1, VELOCITY_MODE)
# ----------------------------------


# STANDUP ROVER


# --------- STATE MACHINE ------------
while True:
    
    try:
        velocity = 1000
        resetGlobalCheck = 0
        driveTrain.goForward(velocity)
        
    except Exception as error:
        print('Caught this error: ' + repr(error))
        if(resetGlobalCheck == 0):
            resetGlobalCheck = 1
            # CHANGE, put reset code here MATTHEW, (call reset)
            # Reset drive train on error