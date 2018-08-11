import Adafruit_PCA9685
pwm = Adafruit_PCA9685.PCA9685()

while True:
	var = raw_input()
	(portStr, angleStr) = var.split(' ')
	port = int(portStr)
	angle = int(angleStr)
	set_pwm(port, 0, angle)