import time

while True:
    command = raw_input("Give Command: ")
    f = open('controlValues.txt','w')
    if len(command) > 0:
        f.write(command)
    f.close()
    time.sleep(.1)