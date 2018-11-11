import smbus
import time
import subprocess
import sys
import os


# LIDAR STUFF -------
from lidar_lite import Lidar_Lite

lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1:
  print "Not Connected"
# ----------



bus = smbus.SMBus(1)

comms900 = 0x04
lastVal = chr(178)

def writeNumber(value):
    try:
        bus.write_byte(comms900, value)
    except IOError:
        print("Failed Write")
        subprocess.call(['i2cdetect', '-y', '1'])

def readString():
    strData = ""
    
    for i in range(0, 32):
        try:
            strData += chr(bus.read_byte(comms900))
            time.sleep(0.05)
        except IOError:
            print("Failed Read")
            subprocess.call(['i2cdetect', '-y', '1'])
            return
    
    time.sleep(0.1)
    return strData

def convertReadStringToIntArray(myStr):
    readStr = readStr.split("_")[0]
    strArray = readStr.split(" ")
    intArray = [int(x) for x in strArray]
    return intArray
    
while True:
    writeNumber(100)
    readStr = readString()
    intArray = convertReadStringToIntArray(readStr)

    print "Lidar info: ", lidar.getDistance(), lidar.getVelocity()

    print("intArray", intArray)

    
    
    time.sleep(.1)
    