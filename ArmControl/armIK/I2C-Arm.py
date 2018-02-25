import smbus
import time
import subprocess
import sys
import os

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

while True:
    writeNumber(100)
    var = readString()
    print(var)
    