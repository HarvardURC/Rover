from picamera.array import PiRGBArray
from picamera import PiCamera
from collections import deque
import numpy as np
import argparse
import imutils
import time
import cv2

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = (20, 86, 6)
greenUpper = (64, 255, 255)
pts = deque(maxlen=32)
counter = 0
(dX, dY) = (0, 0)
direction = ""

# allow the camera to warmup
time.sleep(0.1)

# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
	frame = frame.array
	
	# resize the frame, blur it, and convert it to the HSV
        # color space
        frame = imutils.resize(frame, width=600)
        # blurred = cv2.GaussianBlur(frame, (11, 11), 0)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        # construct a mask for the color "green", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        mask = cv2.inRange(hsv, greenLower, greenUpper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None
        
        # only proceed if at least one contour was found
	if len(cnts) > 0:
	    # find the largest contour in the mask, then use
	    # it to compute the minimum enclosing circle and
	    # centroid
	    c = max(cnts, key=cv2.contourArea)
	    ((x, y), radius) = cv2.minEnclosingCircle(c)
	    M = cv2.moments(c)
	    center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
 
            # only proceed if the radius meets a minimum size
	    if radius > 10:
	    	# draw the circle and centroid on the frame,
	    	# then update the list of tracked points
	    	cv2.circle(frame, (int(x), int(y)), int(radius),
	    		(0, 255, 255), 2)
	    	cv2.circle(frame, center, 5, (0, 0, 255), -1)
	    	pts.appendleft(center)

        # loop over the set of tracked points
	for i in np.arange(1, len(pts)):
	    # if either of the tracked points are None, ignore
	    # them
	    if pts[i - 1] is None or pts[i] is None:
	    	continue

            # check to see if enough points have been accumulated in
	    # the buffer
	    if counter >= 10 and i == 1 and pts[-10] is not None:
		# compute the difference between the x and y
		# coordinates and re-initialize the direction
		# text variables
		dX = pts[-10][0] - pts[i][0]
		dY = pts[-10][1] - pts[i][1]
		(dirX, dirY) = ("", "")

		# ensure there is significant movement in the
		# x-direction
		if np.abs(dX) > 20:
		    dirX = "East" if np.sign(dX) == 1 else "West"

                # ensure there is significant movement in the
		# y-direction
		if np.abs(dY) > 20:
                    dirY = "North" if np.sign(dY) == 1 else "South"

		# handle when both directions are non-empty
		if dirX != "" and dirY != "":
		    direction = "{}-{}".format(dirY, dirX)

		# otherwise, only one direction is non-empty
		else:
		    direction = dirX if dirX != "" else dirY
		    
            # otherwise, compute the thickness of the line and
            # draw the connecting lines
            thickness = int(np.sqrt(32 / float(i + 1)) * 2.5)
            cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

        # show the movement deltas and the direction of movement on
        # the frame
	cv2.putText(frame, direction, (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
		0.65, (0, 0, 255), 3)
	cv2.putText(frame, "dx: {}, dy: {}".format(dX, dY),
		(10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX,
		0.35, (0, 0, 255), 1)

	# show the frame to our screen and increment the frame counter
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
	counter += 1

	# if the 'q' key is pressed, stop the loop
	if key == ord("q"):
	    break
    

	# clear the stream in preparation for the next frame
	rawCapture.truncate(0)

	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break