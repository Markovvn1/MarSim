# 1. Сделать корректное завершение в случае закрытия соединения со стороны мастера

import math
import struct
import socket

__socket = None

def init(addr="localhost"):
	global __socket

	try:
		__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		__socket.connect((addr, 12406))
	except ConnectionRefusedError:
		__socket = None
		raise ConnectionRefusedError("Cannot connect to master. Maybe you use wrong version?")

def deinit():
	global __socket

	if __socket is None:
		return

	__socket.close()
	__socket = None


def _send(t, data=None): # type
	global __socket

	if __socket is None:
		raise RuntimeError("You should call .init() before")

	# send command
	if data is None:
		__socket.send(struct.pack("<HI", t, 0))
	else:
		__socket.send(struct.pack("<HI", t, len(data)) + data)

	# answer
	l = struct.unpack("<I", __socket.recv(4))[0]
	return __socket.recv(l)


def start():
	_send(1)

def stop():
	_send(2)

def isActive():
	return struct.unpack("b", _send(3))[0] != 0


class Robot:

	def __init__(self, x=None, y=None, angle=None):
		if x is None and y is None and angle is None:
			return

		if x is None or y is None or angle is None:
			raise TypeError("you should specify all arguments")

		self.moveTo(x, y, angle)

	def moveTo(self, x, y, angle):
		_send(1024, struct.pack("fff", x, y, angle % (math.pi * 2)))

	def setSpeed(self, motorL, motorR):
		_send(1025, struct.pack("ff", motorL, motorR))

	def addSensor(self, x, y):
		_send(1026, struct.pack("ff", x, y))

	def readSensors(self, format="rgb"):
		ans = _send(1027)

		if format == "rgb":
			return [(ans[i + 0], ans[i + 1], ans[i + 2]) for i in range(0, len(ans), 3)]
		elif format == "gray":
			return [0.3 * ans[i + 0] + 0.59 * ans[i + 1] + 0.11 * ans[i + 2] for i in range(0, len(ans), 3)]
		else:
			raise ValueError("format should be 'rgb' or 'gray'")


import atexit
atexit.register(deinit)