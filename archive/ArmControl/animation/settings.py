
import math

# REAL MOVEMENT SETTINGS
HOME_LEG_POSITIONS = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

TURRET_HOME_POSITIONS = [0,0]

Angles = {
    "l1": math.radians(0),
    "l2": math.radians(-45),
    "base": math.radians(45),
    "wpan": math.radians(0),
    "wtilt": math.radians(0)
}

SERVO_BOUNDS = [(-75.0,75.0),(-80.0,80.0),(-125.0,125.0)]

# static lengths from robot model
L1_LENGTH = 10
L2_LENGTH = 6
HIPHORIZ_LENGTH = 1.75
HIPVERTUP_LENGTH = .75
BASE_WIDTH = 6.25
BASE_LENGTH = 6.25

clawPercent = .8

mode = "free"

MAX_SERVO_SPEED = 200.0

controllerButtonScaleFactor = .2
controllerToggleScaleFactor = .2

# ANIMATION SETTINGS

global t_per_second 
t_per_second = 60

BASE_THICKNESS = 1.0
BASE_RADIUS = 10
L1_HEIGHT = 8
FORWARD_MARKER_THICKNESS = .2

WPAN_LENGTH = 2
WPAN_HEIGHT = 1
WPAN_WIDTH = 2

WTILT_LENGTH = 3

SERVO_UPDATE_DELAY = .01

global ServoPos 
ServoPos = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

global servoGoalPos
servoGoalPos = [[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0],[-45.0, 0.0, 0.0],[45.0, 0.0, 0.0]]

