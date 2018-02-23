import settings as s
from visual import *
import time
import math
from threading import Thread
import threading
import numpy
import random
import math
import os


# helper functions
def vectorAdd(tuple1, tuple2):
	return tuple(map(lambda x, y: x + y, tuple1, tuple2))

def rotateVector(vector, theta):
    a = vector[0]
    b = vector[1]
    L = (a**2.0 + b ** 2.0) ** 0.5
    alpha = math.atan2(b,a)
    return (L * math.cos(alpha + theta), L * math.sin(alpha + theta), vector[2])

def revForV(tuple):
    	return (tuple[0], tuple[2], -tuple[1])

def getVector(length, theta1, baseAngle):
    vec = (length*math.cos(theta1), 0, length*math.sin(theta1))
    rotatedVec = rotateVector(vec, baseAngle)
    return revForV(rotatedVec)

# takes in the x, y, and z position from center of ground
def getIKAnglesFromPos(x,y,z):
    
    # these define the new x and new z values used for the 2DOF IK calculation
    new_x = ((x**2.0 + y**2.0)**.5)
    new_z = -(s.BASE_THICKNESS + s.L1_HEIGHT) + z
    cosl2Angle = (new_x**2.0 + new_z**2.0 - s.L1_LENGTH**2.0 - s.L2_LENGTH**2.0)/(2.0*s.L1_LENGTH*s.L2_LENGTH)

    if abs(cosl2Angle) > 1.0:
        raise Exception('Displacement out of range of servos - Will')

    # get leg joint angles
    l2Angle = math.atan2(-1 * (1.0 - cosl2Angle**2)**.5, cosl2Angle)
    l1Angle = math.atan2(new_z,new_x) - math.atan2(s.L2_LENGTH*math.sin(l2Angle), s.L1_LENGTH + s.L2_LENGTH*math.cos(l2Angle))
    baseAngle = math.atan2(y,x)

    return {"base": baseAngle, "l1": l1Angle, "l2": l2Angle}


# define floor and base
floor = box (pos=revForV((0,0,0)), length=50, height=0.1, width=100, color=color.white)
base = cylinder(pos=revForV((0,0,0)), axis=revForV((0,0,s.BASE_THICKNESS)), radius=s.BASE_RADIUS, color=color.red)
holder = cylinder(pos=revForV((0,0,s.BASE_THICKNESS)), axis=revForV((0,0,s.L1_HEIGHT)), radius=2, color=color.white)
forwardMarker = box (pos=revForV((0,0,s.BASE_THICKNESS + s.FORWARD_MARKER_THICKNESS/2)), height=s.FORWARD_MARKER_THICKNESS, axis=revForV((6,0,0)), width=6, color=color.black)
#cylinder(pos=revForV((0,s.BASE_RADIUS/2,s.BASE_THICKNESS + s.FORWARD_MARKER_THICKNESS/2)), axis=revForV((0,0,s.FORWARD_MARKER_THICKNESS)), radius=.6, color=color.black)

# define arm l1 and arm l2
l1_axis = getVector(s.L1_LENGTH, s.Angles["l1"], s.Angles["base"])
l1 = cylinder(pos=revForV((0,0,s.BASE_THICKNESS + s.L1_HEIGHT)), axis=l1_axis, radius=.4, color=color.blue)

l2_axis = getVector(s.L2_LENGTH, s.Angles["l1"] + s.Angles["l2"], s.Angles["base"])
l2 = cylinder(pos=vectorAdd(l1.pos, l1.axis), axis=l2_axis, radius=.3, color=color.green)


def makeRadian(angles):
    return [math.radians(angles[0]),math.radians(angles[1]), math.radians(angles[2])]

def getCurrentAngles():
    return (30, 40, 50)

def rotateVector(vector, theta):
    a = vector[0]
    b = vector[1]
    L = (a**2.0 + b ** 2.0) ** 0.5
    alpha = math.atan2(b,a)

    return [L * math.cos(alpha + theta), L * math.sin(alpha + theta), vector[2]]

x = 10
y = -5
z = 5


def updateLegsAndBase():
    
    newAngles = getIKAnglesFromPos(x,y,z)
    
    #l1.axis = getVector(s.L1_LENGTH, s.Angles["l1"], s.Angles["base"])
    #l2.axis = getVector(s.L2_LENGTH, s.Angles["l1"] + s.Angles["l2"], s.Angles["base"])

    try:
        l1.axis = getVector(s.L1_LENGTH, newAngles["l1"], newAngles["base"])
        l2.axis = getVector(s.L2_LENGTH, newAngles["l1"] + newAngles["l2"], newAngles["base"])
        l2.pos = vectorAdd(l1.pos, l1.axis)

        forwardMarker.axis = revForV(rotateVector((6,0,0), newAngles["base"]))
    except:
        print("IK out of range")
    


    '''
    # get ideal angles in radian for the visual arrows to update
    ServoPosRadian = [makeRadian(getCurrentAngles(1)),makeRadian(getCurrentAngles(2)),makeRadian(getCurrentAngles(3)),makeRadian(getCurrentAngles(4))]
    # loop through all legs
    for i in range(1,5):
        x_neg = (-1) ** ((i - 1)%3 > 0)
        y_neg = (-1)** math.floor(i/3)

        leg = str(i)

        legs['leg'+leg]['vert'].pos = vectorAdd(base.pos,revForV(rotateVector((x_neg * s.BASE_LENGTH/2.0,y_neg * s.BASE_WIDTH/2.0,-s.BASE_THICKNESS/2.0), s.BaseOrientationAngle)))
        legs['leg'+leg]['vert'].axis = revForV((0,0,s.HIPVERTUP_LENGTH))
        legs['leg'+leg]['horiz'].pos = vectorAdd(legs['leg'+leg]['vert'].pos,legs['leg'+leg]['vert'].axis)
        legs['leg'+leg]['horiz'].axis = revForV(rotateVector((s.HIPHORIZ_LENGTH*cos(ServoPosRadian[i - 1][0]),s.HIPHORIZ_LENGTH*sin(ServoPosRadian[i - 1][0]),0), s.BaseOrientationAngle))
        legs['leg'+leg]['knee2ankle'].pos = vectorAdd(legs['leg'+leg]['horiz'].pos,legs['leg'+leg]['horiz'].axis)
        legs['leg'+leg]['knee2ankle'].axis = revForV(rotateVector((s.L1_LENGTH*cos(ServoPosRadian[i - 1][1])*cos(ServoPosRadian[i - 1][0]),s.L1_LENGTH*cos(ServoPosRadian[i - 1][1])*sin(ServoPosRadian[i - 1][0]),s.L1_LENGTH*sin(ServoPosRadian[i - 1][1])), s.BaseOrientationAngle))
        legs['leg'+leg]['ankle2foot'].pos = vectorAdd(legs['leg'+leg]['knee2ankle'].pos,legs['leg'+leg]['knee2ankle'].axis)
        legs['leg'+leg]['ankle2foot'].axis = revForV(rotateVector((s.L2_LENGTH*cos(ServoPosRadian[i - 1][2])*cos(ServoPosRadian[i - 1][0]),s.L2_LENGTH*cos(ServoPosRadian[i - 1][2])*sin(ServoPosRadian[i - 1][0]),s.L2_LENGTH*sin(ServoPosRadian[i - 1][2])), s.BaseOrientationAngle))

        legs['leg'+leg]['foot'].pos = vectorAdd(legs['leg'+leg]['ankle2foot'].pos,legs['leg'+leg]['ankle2foot'].axis)

        legs['leg'+leg]['vert'].length = s.HIPVERTUP_LENGTH
        legs['leg'+leg]['horiz'].length = s.HIPHORIZ_LENGTH
        legs['leg'+leg]['knee2ankle'].length = s.L1_LENGTH
        legs['leg'+leg]['ankle2foot'].length = s.L2_LENGTH


    base.pos = revForV(s.BasePos)
    base.axis =revForV((math.cos(s.BaseOrientationAngle),math.sin(s.BaseOrientationAngle),0))
    base.length = s.BASE_LENGTH

    TurretPosRadians = [math.radians(s.TurretPos[0]), math.radians(s.TurretPos[1])]
    panBox.axis = revForV((math.cos(s.BaseOrientationAngle + TurretPosRadians[0]),math.sin(s.BaseOrientationAngle + TurretPosRadians[0]),0))
    panBox.pos = numpy.add(base.pos, revForV([0,0,s.PANBOX_HEIGHT/2.0]))
    panBox.length = s.PANBOX_LENGTH


    barrel.pos = numpy.add(panBox.pos, revForV([0,0,s.PANBOX_HEIGHT/2.0]))
    tiltAngle = TurretPosRadians[1]/s.TURRET_GEAR_RATIO
    barrel.axis = revForV((math.cos(tiltAngle) * math.cos(s.BaseOrientationAngle + TurretPosRadians[0] + math.pi/2.0),math.cos(tiltAngle) * math.sin(s.BaseOrientationAngle + TurretPosRadians[0] + + math.pi/2.0), math.sin(tiltAngle)))
    barrel.length = s.BARREL_LENGTH
    if s.isFiring:
        barrel.color = color.red
    else:
        barrel.color = color.white

    directionArrow.pos = [base.pos[0], base.pos[1], base.pos[2] + s.BASE_THICKNESS]
    directionArrow.axis = revForV((math.cos(s.BaseOrientationAngle + math.pi/2.0),math.sin(s.BaseOrientationAngle + math.pi/2.0),0))
    directionArrow.length = s.BASE_LENGTH

    '''
    
    return True




# I copied this code that allows the visualizer to be rotated and zoomed in, using a mouse.
scene.userzoom = False
scene.userspin = False
scene.range = 15

rangemin = .5
rangemax = 40

zoom = False
spin = False

    
scene.forward = revForV([-0.0,1.0,-1.0])

while True:
    rate(s.t_per_second)


    # controls for rotation and such
    if scene.kb.keys:
        k = scene.kb.getkey()
        if k == 'z':
            scene.range = scene.range * 1.5 
        elif k == 'c':
            scene.range = scene.range / 1.5 
        elif k == 'd':
            x += 1
        elif k == 'a':
            x -= 1
        elif k == 'w':
            y += 1
        elif k == 'x':
            y -= 1
        elif k == 's':
            z += 1
        elif k == 'e':
            z -= 1
            
    elif scene.mouse.events:
        m = scene.mouse.getevent()
        if m.drag == 'middle':
            zoom = True
            lasty = m.pos.y
        elif m.drop == 'middle':
            zoom = False
        elif m.drag == 'left':
            spin = True
            lastray = scene.mouse.ray
        elif m.drop == 'left':
            spin = False
    elif zoom:
        newy = scene.mouse.pos.y
        if newy != lasty:
            distance = (scene.center-scene.mouse.camera).mag
            scaling = 10**((lasty-newy)/distance)
            newrange = scaling*scene.range.y
            if rangemin < newrange < rangemax:
                scene.range = newrange
                lasty = scaling*newy
    elif spin:
        newray = scene.mouse.ray
        dray = newray-lastray
        right = scene.forward.cross(scene.up).norm() # unit vector to the right
        up = right.cross(scene.forward).norm() # unit vector upward
        anglex = -4*arcsin(dray.dot(right))
        newforward = vector(scene.forward)
        newforward = rotate(newforward, angle=anglex, axis=scene.up)
        newray = rotate(newray, angle=anglex, axis=scene.up)
        angley = 4*arcsin(dray.dot(up))
        maxangle = scene.up.diff_angle(newforward)
        if not (angley >= maxangle or angley <= maxangle-pi):
            newforward = rotate(newforward, angle=angley, axis=right)
            newray = rotate(newray, angle=angley, axis=right)
        scene.forward = newforward
        lastray = newray


# -------------------------------------------

    #base.pos = revForV(0,0,0)

    # UPDATE LEGS AND BASE
    updateLegsAndBase()

    #s.Angles["l1"] += math.radians(.3)
    #s.Angles["base"] += math.radians(.2)
    #s.Angles["l2"] += math.radians(-.2)

