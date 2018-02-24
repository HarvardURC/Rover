'''
Wrist tilt impact limit 300, max  600
Wrist pan 130 to 600


continuous servo 380 is stop 500 go 200 go other waz
'''

import math

#CONSTANTS
'''
We are looking at the 2dimensional projection of the arm

x = 0 is the x position of the center of l1's axis
z = 0 is the z position of the center of l1's axis

l1firstJointPos point is the x,z position of the l1 axis first joint
l2firstJointPos point is the x,z position of the l2 axis first joint

l1EndJoint is 
'''

l1firstJointPos = (2.0,-2.0)
l1parallelToJoint = 12.0
l1PerpToJoint = 1.0

l2firstJointPos = (-2.4,-.9)
l2parallelToJoint = 7.625
l2PerpToJoint = 1.0

L1_LENGTH = 14.5
L2_LENGTH = 11.0

Actuator1 = {
"maxlen": 15.0,
"minlen": 12.0,
"maxpos": 200,
"minpos": 500}

Actuator2 = {
"maxlen": 12.0,
"minlen": 8.0,
"maxpos": 240,
"minpos": 500}


def mag(x,z):
    return (x**2 + z**2)**.5


# finds Length of arm necessary to make arm be at theta for l1
# theta1 defined as angle ABOVE line z = 0
def getLengthFromTheta_l1(theta1):
    alpha = math.atan2(l1PerpToJoint,l1parallelToJoint)

    lengthToJoint = mag(l1PerpToJoint, l1parallelToJoint)

    desiredX = lengthToJoint*math.cos(alpha + theta1)
    desiredZ = lengthToJoint*math.sin(alpha + theta1)

    desiredL = mag(desiredX - l1firstJointPos[0], desiredZ - l1firstJointPos[1])

    return desiredL


# finds Length of arm necessary to make arm be at theta2 for l2
# theta2 here is defined as angle BELOW the line of l1
# theta2 here is always negative

def getLengthFromTheta_l2(theta2):
    beta = math.atan2(l2PerpToJoint,l2parallelToJoint)

    lengthToJoint = mag(l2PerpToJoint, l2parallelToJoint)

    desiredX = lengthToJoint*math.cos(beta + theta2)
    desiredZ = -1 * lengthToJoint*math.sin(beta + theta2)

    desiredL = mag(desiredX - l2firstJointPos[0], desiredZ - l2firstJointPos[1])

    return desiredL


# takes in the x, z position and returns angles to get there
def getIKAnglesFromPos(x,z):
    
    # these define the new x and new z values used for the 2DOF IK calculation
    new_x = x
    new_z = z
    cosl2Angle = (new_x**2.0 + new_z**2.0 - L1_LENGTH**2.0 - L2_LENGTH**2.0)/(2.0*L1_LENGTH*L2_LENGTH)

    if abs(cosl2Angle) > 1.0:
        raise Exception('Displacement out of range of servos - Will')

    # get leg joint angles
    l2Angle = math.atan2(-1 * (1.0 - cosl2Angle**2)**.5, cosl2Angle)
    l1Angle = math.atan2(new_z,new_x) - math.atan2(L2_LENGTH*math.sin(l2Angle), L1_LENGTH + L2_LENGTH*math.cos(l2Angle))

    return (l1Angle, l2Angle)

def getLengthsFromPos(x,z):
    angles = getIKAnglesFromPos(x,z)
    return (getLengthFromTheta_l1(angles[0]), getLengthFromTheta_l2(angles[1]))


def getActuatorPosFromLength(l, actuator = 1):
    if actuator == 1:
        a = Actuator1
    elif actuator == 2:
        a = Actuator2

    ratio = (a["maxpos"] - a["minpos"])/(a["maxlen"] - a["minlen"])

    if l > a["maxlen"] or l < a["minlen"]:
        raise Exception("Told to go to length that the actuator cannot")
    
    deltaLen = l - a["minlen"]
    newPos = a["minpos"] + deltaLen * ratio

    return newPos
    

lengths = getLengthsFromPos(12,4)
print lengths
print getActuatorPosFromLength(lengths[0], 1)
print getActuatorPosFromLength(lengths[1], 2)
