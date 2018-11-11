import smbus
import time
bus = smbus.SMBus(1)

comms900 = 0x04
comms433 = 0x05
usingBackupComms = False

def writeNumber(value):
    
    if not usingBackupComms:
        bus.write_byte(comms900, value)
        
    else:
        bus.write_byte(comms433, value)
    
    return -1

def readNumber():
    value = -1
    
    if not usingBackupComms:
        value = bus.read_byte(comms900)
        
    else:
        value = bus.read_byte(comms433)
    
    return number

while True:
    var = readNumber()
    
    if not var:
        print("No Data - Send start command from LoRa")
        time.sleep(1)
        continue
    
    print( "Got", var)
    time.sleep(0.5)
