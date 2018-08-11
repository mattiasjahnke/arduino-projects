import serial
import pyautogui

arduino = serial.Serial('/dev/cu.usbmodem1461', 115200, timeout=.1)

pyautogui.PAUSE = 0

keysDown = {}

# Element = key to be pressed
# leave empty to not map
keyMap = [
	'A',		# A
	'B',		# B
	'space',	# C
	'D',		# D
	'E',		# E
	'F',		# F
	'',			# JoyStick button
]

def keyDown(key):
	if key not in keysDown:
		keysDown[key] = True
		pyautogui.keyDown(key)
		# print('Down: ', key)


def keyUp(key):
	if key in keysDown:
		del(keysDown[key])
		pyautogui.keyUp(key)
		# print('Up: ', key)


def handleJoyStickAsArrowKeys(x, y):
	if x > 0:
		keyDown('right')
		keyUp('left')
	elif x < 0:
		keyDown('left')
		keyUp('right')
	else:
		keyUp('left')
		keyUp('right')

	if y > 0:
		keyDown('down')
		keyUp('up')
	elif y < 0:
		keyDown('up')
		keyUp('down')
	else:
		keyUp('up')
		keyUp('down')


def handleButtonState(state):
	for i in range(7):
		if not keyMap[i]:
			continue

		if buttonState >> i & 1:
			keyDown(keyMap[i])
		else:
			keyUp(keyMap[i])


while True:
	data = arduino.read()
	if data == 'S':
		dx = int(arduino.read_until(',')[:-1])
		if abs(dx) < 10:
			dx = 0
		dy = int(arduino.read_until(',')[:-1])
		if abs(dy) < 10:
			dy = 0
		buttonState = int(arduino.readline()[:-2])
		handleJoyStickAsArrowKeys(dx, dy * -1)
		handleButtonState(buttonState)




