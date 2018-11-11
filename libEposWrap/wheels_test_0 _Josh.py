import DriveTrain

speed = 1200 

driveTrain = DriveTrain.EposDriveTrain()
driveTrain.init()
driveTrain.enableAll()
driveTrain.clearAllFaults()
VELOCITY_POSITION_MODE = 1
for i in range(6):
    driveTrain.setMode(i + 1, VELOCITY_POSITION_MODE)
    driveTrain.setVelocity(i+1, 0)

def move(leftSpeed, rightSpeed):
    for i in range(3):
        driveTrain.setVelocity(i+1, leftSpeed)
        driveTrain.setVelocity(i+4, rightSpeed)
        
def stop():
    for i in range(6):
        driveTrain.setVelocity(i+1, 0) 


move(speed, speed)