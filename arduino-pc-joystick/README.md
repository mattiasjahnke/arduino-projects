# arduino-pc-joystick

Source code for an arduino gamepad using the Funduino JoyStick Shield.

### Components used:
* Arduino Uno
* Funduino JoyStick Shield

## Instructions

1) Upload the Arduino sketch to an Arduino with a JoyStick shield attached to it.
2) Modify `Funduino-JoyStick.py` with your Arduinos port name (for example `/dev/cu.usbmodem1461`)
3) Run `python Funduino-JoyStick.py`

## How it works

### Arduino

Gathers the button states (7 buttons + joystick x/y) and constructs a data frame. This frame is sent via Serial over to a PC running a python script.

### Python

The script is listening for incoming data from the Arduino. Whenever it detects a new frame, it will extract the values for joystick x/y and all the button states. It will convert the button pushed to computer input using the library `pyautogui`.

According to `pyautogui`, it should work cross platform. I have however only tested on MacOS.
