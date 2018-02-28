import time
import math
import serial

ser = serial.Serial('/dev/cu.usbmodem1421', 9600)                

while True:
    time.sleep(2)
    sendString = str(input("give string of 6 ints: "))
    ser.write(sendString.encode())


        
        
        
    

    