import constants

def getMoveCommandInfo(curMovement, state):
    #legAngles[6] int legSpeeds[6] bool goClockwises[6]
    a = 2*pi - landingAngle
    b = landingAngles
    air = legAirSpeed
    ground = legGroundSpeed

    if curMovement == 'STANDUP':
        goClockwises = [True, True, True, True, True, True]
        legAngles = [0, 0, 0, 0, 0, 0]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'FORWARD':
        goClockwises = [True, True, True, True, True, True]
        if state == 1:
            legAngles = [b, a, b, a, b, a]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif state == 2:
            legAngles = [a, b, a, b, a, b]
            legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'BACKWARD':
        goClockwises = [False, False, False, False, False, False]
        if state == 1:
            legAngles = [a, b, a, b, a, b]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif state == 2:
            legAngles = [b, a, b, a, b, a]
            legSpeeds = [air, ground, air, ground, air, ground]

    elif curMovement == 'ROTATECLOCKWISE': 
        goClockwises = [True, True, True, False, False, False]
        if (state == 1):
            legAngles = [b, a, b, b, a, b]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif (state == 2):
            legAngles = [a, b, a, a, b, a]
            legSpeeds = [air, ground, air, ground, air, ground]

    elif curMovement == 'ROTATECOUNTERCLOCKWISE': 
        goClockwises = [False, False, False, True, True, True]
        if (state == 1):
            legAngles = [a, b, a, a, b, a]
            legSpeeds = [ground, air, ground, air, ground, air]
        elif (state == 2):
            legAngles = [b, a, b, b, a, b]
            legSpeeds = [air, ground, air, ground, air, ground]

    # setup object
    m = {}
    m["legAngles"] = legAngles
    m["legSpeeds"] = legSpeeds
    m["goClockwises"] = goClockwises

    return m


def getSetupInfo(curMovement, curPos):
    if curMovement == 'FORWARD':
        goClockwises = [True, True, True, True, True, True]
        legAngles = [a, b, a, b, a, b]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'BACKWARD':
        goClockwises = [False, False, False, False, False, False]
        legAngles = [b, a, b, a, b, a]
        legSpeeds = [air, air, air, air, air, air]
    elif curMovement == 'ROTATECLOCKWISE':
        goClockwises = [True, True, True, False, False, False]
        legAngles = [a, b, a, a, b, a]
        legSpeeds = [air, ground, air, ground, air, ground]
    elif curMovement == 'ROTATECOUNTERCLOCKWISE':
        goClockwises = [False, False, False, True, True, True]
        legAngles = [b, a, b, b, a, b]
        legSpeeds = [air, ground, air, ground, air, ground]
    
    # setup object
    m = {}
    m["legAngles"] = legAngles
    m["legSpeeds"] = legSpeeds
    m["goClockwises"] = goClockwises

    return m



''' USING C++ VERSION OF THIS FUNCTION

  # converts curPos (absolute maxon coordinates) and goalAngle (radians) into goalPos (maxon coordinates)
  # goalAngle for all legs is defined to be zero if pointed down and pi/2 if pointing toward back of rover
def getGoalPos(legID, curPos, goalAngle, rotClockwise):
    # get whether the leg is one of the legs on the right side of the rover
    isRightSideLeg = ((legID == FRONTRIGHT) or (legID == MIDDLERIGHT) or (legID == BACKRIGHT))
      
    # goalAngleMaxonCoord is the goal angle in maxon coordinates where 0 is pointing down and MODVALUE/4 is pointing
    # toward the back of the rover
    goalAngleMaxonCoord = int(MODVALUE * goalAngle / (2*pi))
    
    # adjust curPos to account for the offset angle from zero that all motors started at
    # its different for right and left legs because of opposite orientation of the right side motors
    # and the left side motors. Left side motors rotate clockwise toward the front when the maxon coordinates 
    # are increasing
    originalCurPos = curPos
    
    if not isRightSideLeg:
        curPos = curPos + int(START_OFFSET_ANGLE * MODVALUE / (2 * pi))
    else:
        curPos = curPos - int(START_OFFSET_ANGLE * MODVALUE / (2 * pi))
    
    # get the current angle of the leg in terms of maxon coordinates modded to between 0 and MODVALUE
    # this depends on whether the 
    if (curPos >= 0) and (!isRightSideLeg):
        curAngleMaxonCoord = (curPos % MODVALUE)
    else if (curPos < 0) and (!isRightSideLeg):
        curAngleMaxonCoord = MODVALUE - (-curPos % MODVALUE)
    else if (curPos >= 0) and (isRightSideLeg):
        curAngleMaxonCoord =  -1 * (curPos % MODVALUE) + MODVALUE
    else if (curPos < 0) and (isRightSideLeg):
        curAngleMaxonCoord = (-curPos % MODVALUE)
    
    diff = goalAngleMaxonCoord - curAngleMaxonCoord
    
    # Calculate delta (maxon coordinates) that we want to add to the current position
    # Note that if a leg is told to go clockwise to its current position, it 
    if rotClockwise:
        if diff > 0:
            delta = diff
        else:
            delta = diff + MODVALUE
    else:
        if diff > 0:
            delta = diff - MODVALUE
        else:
            delta = diff

    # if legID is one of the right legs then they are physically oriented such
    # that clockwise motion is negative. Therefore must multiply delta by a factor of -1
    if isRightSideLeg:
        delta = -1 * delta
    
    return (originalCurPos + delta)

'''
