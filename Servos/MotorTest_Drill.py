import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO.setup(19, GPIO.OUT)

while True:
    GPIO.output(19,0)
    time.sleep(2)
    GPIO.output(19,1)
    time.sleep(2)