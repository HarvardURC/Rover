import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(19, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)

# 16 is direction1, 19 is direction2

GPIO.output(19, False)
GPIO.output(16, True)

time.sleep(3)

GPIO.output(19, False)
GPIO.output(16, False)

time.sleep(3)

GPIO.output(19, True)
GPIO.output(16, False)

time.sleep(3)

GPIO.output(19, False)
GPIO.output(16, False)

GPIO.cleanup()
            
