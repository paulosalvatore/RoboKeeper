# USAGE
# python object_movement.py --video object_tracking_example.mp4
# python object_movement.py

# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2

from serial import *
import threading


def receiving(ser):
	while True:
		leitura = ser.read(ser.inWaiting())

		leituraClean = leitura.decode("utf-8")

		if leituraClean != "":
			print("Recebimento na Porta Serial: {}".format(leituraClean))

anguloRoboZerado = 90
anguloRobo = anguloRoboZerado
raioAtual = 0
raioMinimo = 20


def gravar():
	if raioAtual >= raioMinimo:
		ser.write(anguloRobo)
	else:
		ser.write(str(anguloRoboZerado).encode())


def set_interval(func, sec):
	def func_wrapper():
		set_interval(func, sec)
		func()

	t = threading.Timer(sec, func_wrapper)
	t.start()

	return t

"""
if __name__ == '__main__':
	ser = Serial(
		port="COM19",
		baudrate=9600,
		bytesize=EIGHTBITS,
		parity=PARITY_NONE,
		stopbits=STOPBITS_ONE,
		timeout=0.1,
		xonxoff=0,
		rtscts=0,
		interCharTimeout=None
	)

	threading.Thread(target=receiving, args=(ser,)).start()

	set_interval(gravar, 0.5)
"""

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=32, help="max buffer size")
args = vars(ap.parse_args())

xMax = 600
yMax = 450

# define the lower and upper boundaries of the "green"
# ball in the HSV color space
# greenLower = (29, 86, 6)
# greenUpper = (64, 255, 255)
# greenLower = (0, 128, 99)
# greenUpper = (255, 255, 255)
# greenLower = (168, 151, 107)
# greenUpper = (255, 255, 255)
greenLower = (25, 48, 135)
greenUpper = (255, 255, 255)

# initialize the list of tracked points, the frame counter,
# and the coordinate deltas
pts = deque(maxlen=args["buffer"])
counter = 0
(dX, dY) = (0, 0)
direction = ""

# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False):
	camera = cv2.VideoCapture(0)

# otherwise, grab a reference to the video file
else:
	camera = cv2.VideoCapture(args["video"])

# keep looping
while True:
	# grab the current frame
	(grabbed, frame) = camera.read()

	# if we are viewing a video and we did not grab a frame,
	# then we have reached the end of the video
	if args.get("video") and not grabbed:
		break

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

	x = 0
	y = 0

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
			cv2.circle(
				frame,
				(int(x), int(y)),
				int(radius),
				(0, 255, 255),
				2
			)

			raioAtual = radius

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
		if len(pts) >= 10:
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
		thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
		cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

	# show the movement deltas and the direction of movement on
	# the frame
	cv2.putText(
		frame,
		direction,
		(10, 30),
		cv2.FONT_HERSHEY_SIMPLEX,
		0.65,
		(0, 0, 255),
		3
	)

	xReal = int(x - (xMax / 2))
	yReal = int(yMax - y)

	# xReal = int(x)
	# yReal = int(y)

	cv2.putText(
		frame,
		"xReal: {}, yReal: {}".format(xReal, yReal),
		(10, frame.shape[0] - 10),
		cv2.FONT_HERSHEY_SIMPLEX,
		0.35,
		(0, 0, 255),
		1
	)

	def angle_between(p1, p2):
		ang1 = np.arctan2(*p1[::-1])
		ang2 = np.arctan2(*p2[::-1])
		return np.rad2deg((ang1 - ang2) % (2 * np.pi))

	ponto1 = (0, 0)
	ponto2 = (xReal, yReal)

	angulo = angle_between(ponto1, ponto2) - 180

	anguloRobo = str(int(angulo)).encode()

	cv2.putText(
		frame,
		"Angulo: {}".format(anguloRobo),
		(200, frame.shape[0] - 10),
		cv2.FONT_HERSHEY_SIMPLEX,
		0.35,
		(0, 0, 255),
		1
	)

	cv2.putText(
		frame,
		"Raio: {}".format(raioAtual),
		(300, frame.shape[0] - 10),
		cv2.FONT_HERSHEY_SIMPLEX,
		0.35,
		(0, 0, 255),
		1
	)

	# show the frame to our screen and increment the frame counter
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
	counter += 1

	# if the 'q' key is pressed, stop the loop
	if key == ord("q"):
		break

# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
