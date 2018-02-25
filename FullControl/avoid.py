import time
import random
import math
from testLidarSweep import testLidarSweep

class main():
	def __init__(self):
		self.sweepdegrees = LidarSweep.getData()[0]
        self.lidarValues = LidarSweep.get_lidarValues()[1]
        self.numVals = len(self.lidarValues)
        self.center = math.ceil(self.numVals/2)
        self.minDist = 1

    # filter function to remove noise and irrelevent readings
	def filter(self):
		self.lidarValues = self.lidarValues

	def turnDirection(self):
		rl = [0,0]
		best_index = armgmax([sum(lidarValues[:self.center]), sum(lidarValues[self.center:])])
		rl[best_index] = 1
		return rl

	def control(self):
		stable = True
    	
		# determine whether or not to turn
		if self.lidarValues[self.center] < minDist:
			self.stable = False

		# determine turning until clear to go forward
		while (stable == False):
			right = self.turnDirection()[0]
			left = self.turnDirection()[1]

			if self.lidarValues[self.center] < minDist:
				self.stable = True

		forward = int(stable == True)
		return [forward, right, left]

m = main()
m.control()
