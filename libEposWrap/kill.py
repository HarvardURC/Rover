# -*- coding: utf-8 -*-
"""
Created on Sat Nov 10 19:06:11 2018

@author: Jenny
"""
import DriveTrain
driveTrain = DriveTrain.EposDriveTrain()
driveTrain.init()
driveTrain.enableAll()
driveTrain.clearAllFaults()
VELOCITY_POSITION_MODE = 0
for i in range(6):
    driveTrain.setMode(i + 1, VELOCITY_POSITION_MODE)
    driveTrain.setVelocityProfile(i + 1, 0, 0);
    driveTrain.setVelocity(i+1, 0)