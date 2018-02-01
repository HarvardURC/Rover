import settings as s
from visual import *
import pygame
import time
import numpy
import random
import math
print (pygame.__file__)
'''
Gets joystick data and prints it
'''

pygame.init()
x = pygame.joystick.get_count()
print("joystick count:", x)

j = pygame.joystick.Joystick(0)
j.init()
print ('Initialized Joystick : %s' % j.get_name())

which_toggle = ["LeftToggle_horizontal", "LeftToggle_vertical", "RightToggle_horizontal", "RightToggle_vertical",]
which_button = ["button_X", "button_A", "button_B", "button_Y", "button_LB", "button_RB", "button_LT", "button_RT", "button_back", "button_start", "button_toggleleft", "button_toggleright"]



def revForV(v):
    if type(v) == tuple:
        return vector(v[0], v[2], -v[1])
    else:
        save = v[2]
        v[2] = -v[1]
        v[1] = save
        return v

xVec = revForV(vector(1,0,0))
yVec = revForV(vector(0,1,0))
zVec = revForV(vector(0,0,1))

def normVec(v):
    m = mag(v)
    return v/m

# gets the vector of length and theta1 relative to ground and with base rotated baseAngle
def getVector(length, theta1, baseAngle):
    v = revForV(vector(length,0,0))
    v = v.rotate(angle=theta1, axis=revForV(vector(0,1,0)))
    v = v.rotate(angle=baseAngle, axis=revForV(vector(0,0,1)))
    return v

# takes in the x, y, and z position from center of ground
def getIKAnglesFromPos(x,y,z):
    # these define the new x and new z values used for the 2DOF IK calculation
    new_x = ((x**2.0 + y**2.0)**.5)
    new_z = -(s.BASE_THICKNESS + s.L1_HEIGHT) + z
    cosl2Angle = (new_x**2.0 + new_z**2.0 - s.L1_LENGTH**2.0 - s.L2_LENGTH**2.0)/(2.0*s.L1_LENGTH*s.L2_LENGTH)

    if abs(cosl2Angle) > 1.0:
        return False

    # get leg joint angles
    l2Angle = math.atan2(-1 * (1.0 - cosl2Angle**2)**.5, cosl2Angle)
    l1Angle = math.atan2(new_z,new_x) - math.atan2(s.L2_LENGTH*math.sin(l2Angle), s.L1_LENGTH + s.L2_LENGTH*math.cos(l2Angle))
    baseAngle = math.atan2(y,x)

    return {"base": baseAngle, "l1": l1Angle, "l2": l2Angle}



# define floor and base
floor = box (pos=revForV((0,0,0)), length=50, height=0.1, width=100, color=color.white)
base = cylinder(pos=revForV((0,0,0)), axis=revForV((0,0,s.BASE_THICKNESS)), radius=s.BASE_RADIUS, color=color.red)
holder = cylinder(pos=revForV((0,0,s.BASE_THICKNESS)), axis=revForV((0,0,s.L1_HEIGHT)), radius=2, color=color.white)
#forwardMarker = box (pos=revForV((0,0,s.BASE_THICKNESS + s.FORWARD_MARKER_THICKNESS/2)), height=s.FORWARD_MARKER_THICKNESS, axis=revForV((6,0,0)), width=6, color=color.black)


# define arm l1 and arm l2
l1_axis = getVector(s.L1_LENGTH, s.Angles["l1"], s.Angles["base"])
l1 = cylinder(pos=revForV((0,0,s.BASE_THICKNESS + s.L1_HEIGHT)), axis=l1_axis, radius=.4, color=color.blue)

l2Angle = s.Angles["l1"] + s.Angles["l2"]
l2_axis = getVector(s.L2_LENGTH, l2Angle, s.Angles["base"])
l2 = cylinder(pos=l1.pos + l1.axis, axis=l2_axis, radius=.3, color=color.green)

wpan_axis = getVector(s.WPAN_LENGTH, l2Angle, s.Angles["base"])
wpan = box (pos=l2.pos + l2.axis, height=s.WPAN_HEIGHT, width=s.WPAN_WIDTH, axis=revForV((0,0,-1)), color=color.blue)
#arrowHelp = arrow(pos=(0,2,1), axis=(5,0,0), shaftwidth=1)

wtilt1 = cylinder (pos=l2.pos + l2.axis, axis=l2_axis, radius=.3, color=color.blue)
wtilt2 = cylinder (pos=l2.pos + l2.axis, axis=l2_axis, radius=.3, color=color.blue)

x = 10
y = 9
z = 5



def updateLegsAndBase():
    
    
    newAngles = getIKAnglesFromPos(x,y,z)
    if newAngles == False:
        return

    # negative because vpython rotates these clockwise for some reason
    s.Angles["l1"] = newAngles["l1"]
    s.Angles["l2"] = newAngles["l2"]
    s.Angles["base"] = newAngles["base"]

    if s.mode == "clawDown":
        s.Angles["wtilt"] =  -(s.Angles["l1"] + s.Angles["l2"]) -math.pi/2.0
        s.Angles["wpan"] = 0

    # draw objects
    l1Angle = -s.Angles["l1"]
    l2Angle = -s.Angles["l2"]
    baseAngle = newAngles["base"]
    wtiltAngle = -s.Angles["wtilt"]
    wpanAngle = -s.Angles["wpan"]


    l1.axis = getVector(s.L1_LENGTH, l1Angle, baseAngle)
    armAngle = l1Angle + l2Angle
    l2.axis = getVector(s.L2_LENGTH, armAngle, baseAngle)
    l2.pos = l1.pos + l1.axis


    #forwardMarker.axis = rotateVector(revForV((6,0,0)), s.Angles["base"])


    # pan stuff
    wpan.pos = l2.pos + normVec(l2.axis)* (s.L2_LENGTH + s.WPAN_HEIGHT/2.0)

    baseVector = xVec.rotate(angle=baseAngle, axis=zVec)

    # w is the vector in plane of arm and is parallel to wpan and through wpan center
    c = cross(l2.axis, cross(baseVector, zVec))
    w = c.rotate(angle=wpanAngle, axis=l2.axis)

    # tiltOffset is the vector perp to the plane of the arm and parallel to wpan and through wpan center
    tiltOffset = cross(baseVector, zVec).rotate(angle=wpanAngle, axis=l2.axis)

    wpan.axis = normVec(w) * s.WPAN_WIDTH
    wpan.up = -l2.axis
    
    #arrowHelp = arrow(pos=wpan.pos, axis=w, shaftwidth=.3, color=color.white)
    #arrowHelp = arrow(pos=wpan.pos, axis=l2.axis, shaftwidth=.3, color=color.green)


    # tilt stuff
    centerPos = l2.pos + normVec(l2.axis) * (s.L2_LENGTH + s.WPAN_HEIGHT)
    wtilt1.pos = centerPos + normVec(tiltOffset) * s.WPAN_HEIGHT * s.clawPercent
    wtilt2.pos = centerPos - normVec(tiltOffset) * s.WPAN_HEIGHT * s.clawPercent
    unrotatedWtilt = getVector(s.WTILT_LENGTH, armAngle + wtiltAngle, baseAngle)
    rotatedWithtilt = unrotatedWtilt.rotate(angle=wpanAngle, axis=l2.axis)
    wtilt1.axis = rotatedWithtilt
    wtilt2.axis = rotatedWithtilt

    print((x,y,z), base.axis + holder.axis + l1.axis + l2.axis)
   
    





# I copied this code that allows the visualizer to be rotated and zoomed in, using a mouse.
scene.userzoom = False
scene.userspin = False
scene.range = 20

rangemin = .5
rangemax = 40

zoom = False
spin = False

    
scene.forward = revForV([-0.0,1.0,-1.0])

while True:
    rate(s.t_per_second)


    # controls for rotation and such
    #if scene.kb.keys:
        #k = scene.kb.getkey()
            
    if scene.mouse.events:
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


    
    # UPDATE LEGS AND BASE
    updateLegsAndBase()

    
    pygame.event.pump()

    events = pygame.event.get()
    for event in events:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                scene.range = scene.range * 1.5 
                print("left")
            elif event.key == pygame.K_RIGHT:
                scene.range = scene.range / 1.5 
                print("right")
            elif event.key == pygame.K_m:
                if s.mode == "clawDown":
                    s.mode = "free"
                else:
                    s.mode = "clawDown"

        # read input from the two joysticks   
    for i in range(0, j.get_numaxes()):
        # make sure toggle is past a certain cutoff angle
        if abs(j.get_axis(i)) >= 0.07:
            if which_toggle[i] == "RightToggle_vertical":
                y += -j.get_axis(i) * s.controllerToggleScaleFactor
            elif which_toggle[i] == "RightToggle_horizontal":
                x += j.get_axis(i) * s.controllerToggleScaleFactor
            elif which_toggle[i] == "LeftToggle_vertical":
                z += -j.get_axis(i) * s.controllerToggleScaleFactor
            elif which_toggle[i] == "LeftToggle_vertical":
                s.Angles["base"] += -j.get_axis(i) * s.controllerToggleScaleFactor
                
    
    for i in range(0, j.get_numbuttons()):
        if j.get_button(i) != 0:
            print (which_button[i], "button Joystick Num:", i)
            if which_button[i] == "button_A":
                s.clawPercent -= .1 * s.controllerButtonScaleFactor
            elif which_button[i] == "button_Y":
                s.clawPercent += .1 * s.controllerButtonScaleFactor
            elif which_button[i] == "button_RB":
                s.Angles["wpan"] += .1 * s.controllerButtonScaleFactor
            elif which_button[i] == "button_LB":
                s.Angles["wpan"] -= .1 * s.controllerButtonScaleFactor
            elif which_button[i] == "button_RT":
                s.Angles["wtilt"] += .1 * s.controllerButtonScaleFactor
            elif which_button[i] == "button_LT":
                s.Angles["wtilt"] -= .1 * s.controllerButtonScaleFactor
                

#which_toggle = ["LeftToggle_horizontal", "LeftToggle_vertical", "RightToggle_horizontal", "RightToggle_vertical",]
#which_button = ["button_X", "button_A", "button_B", "button_Y", "button_LB", "button_RB", "button_LT", "button_RT", "button_back", "button_start", "button_toggleleft", "button_toggleright"]


    

