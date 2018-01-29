
import math

# REAL MOVEMENT SETTINGS
HOME_LEG_POSITIONS = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

TURRET_HOME_POSITIONS = [0,0]

Angles = {
    "l1": math.radians(45),
    "l2": math.radians(0),
    "base": math.radians(45)
}

SERVO_BOUNDS = [(-75.0,75.0),(-80.0,80.0),(-125.0,125.0)]

# static lengths from robot model
L1_LENGTH = 10
L2_LENGTH = 6
HIPHORIZ_LENGTH = 1.75
HIPVERTUP_LENGTH = .75
BASE_WIDTH = 6.25
BASE_LENGTH = 6.25




MAX_SERVO_SPEED = 200.0

# ANIMATION SETTINGS

global t_per_second 
t_per_second = 30

BASE_THICKNESS = 1.0
BASE_RADIUS = 10
L1_HEIGHT = 8
FORWARD_MARKER_THICKNESS = .2

SERVO_UPDATE_DELAY = .01

global ServoPos 
ServoPos = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

global servoGoalPos
servoGoalPos = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

# turret servo current pos and goal pos
global TurretPos
TurretPos = [0.0, 0.0]
global turretServoGoalPos
turretServoGoalPos = [0.0, 0.0]

global draggingLegs
draggingLegs = [False,False,False,False]

# made for individual legs so that we could average them together
global legBasePos 
#legBasePos = [[0,0,-HOMEPOS_FOOTHEIGHT + BASE_THICKNESS/2.0 + FOOT_RADIUS],[0,0,-HOMEPOS_FOOTHEIGHT + BASE_THICKNESS/2.0 + FOOT_RADIUS],[0,0,-HOMEPOS_FOOTHEIGHT + BASE_THICKNESS/2.0 + FOOT_RADIUS],[0,0,-HOMEPOS_FOOTHEIGHT + BASE_THICKNESS/2.0 + FOOT_RADIUS]]

global BasePos 
#BasePos = [0,0,-HOMEPOS_FOOTHEIGHT + BASE_THICKNESS/2.0 + FOOT_RADIUS]

global BaseOrientationAngle
#BaseOrientationAngle = 0.0
