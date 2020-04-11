from marsim import rescue_line


rescue_line.init()

robot = rescue_line.Robot()
# robot = rescue_line.Robot(x=0.4, y=0.3, angle=0)
robot.addSensor(0.01, 0.14) # add one sensor

rescue_line.start() # run simulation (optional)

while True:
	s1 = robot.readSensors("gray")[0] # get information from first sensor
	u = (s1 - 128) / 30 # use this information to calculate control coefficient
	robot.setSpeed(6 + u, 6 - u) # set speed to motors

rescue_line.stop() # stop simulation (optional)