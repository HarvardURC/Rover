from lidarControl import Lidar_Lite
import accelerometerControl as acc

import smbus
import time


# LIDAR SETUP -----------------
lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1: 
    print "Not Connected" 

# -----------------------------

while True: 
    # LIDAR LOOP -----------------
    print lidar.getDistance() 
    print lidar.getVelocity() 
    # ----------------------------

    # ACCELEROMETER LOOP -----------------------------
    # Read the Euler angles for heading, roll, pitch (all in degrees). 
    heading, roll, pitch = acc.bno.read_euler() 
    # Read the calibration status, 0=uncalibrated and 3=fully calibrated. 
    sys, gyro, accel, mag = acc.bno.get_calibration_status() 

    # Print everything out. 
    print('Heading={0:0.2F} Roll={1:0.2F} Pitch={2:0.2F}\tSys_cal={3} Gyro_cal={4} Accel_cal={5} Mag_cal={6}'.format( heading, roll, pitch, sys, gyro, accel, mag))

    # ----------------------------------------------

    time.sleep(.5)