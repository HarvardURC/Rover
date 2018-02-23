from lidar_lite import Lidar_Lite

import smbus
import time

# Get I2C buss
bus = smbus.SMBus(1)
bus.write_byte(0x40, 0xF5)

lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1:
  print "Not Connected"

print lidar.getDistance()
print lidar.getVelocity()