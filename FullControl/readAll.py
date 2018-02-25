from lidarControl import Lidar_Lite

import smbus
import time

import accelerometerControl as acc

lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1:
  print "Not Connected"

while True:    
  # get lidar data
  print lidar.getDistance()
  print lidar.getVelocity()

  # Read the Euler angles for heading, roll, pitch (all in degrees).
  heading, roll, pitch = bno.read_euler()
  # Read the calibration status, 0=uncalibrated and 3=fully calibrated.
  sys, gyro, accel, mag = bno.get_calibration_status()
  # Print everything out.
  print('Heading={0:0.2F} Roll={1:0.2F} Pitch={2:0.2F}\tSys_cal={3} Gyro_cal={4} Accel_cal={5} Mag_cal={6}'.format(
          heading, roll, pitch, sys, gyro, accel, mag))