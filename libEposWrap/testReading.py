import time

while True:
    time.sleep(.3)
    f = open('controlValues.txt','r') 
    getValue = f.read()
    print getValue