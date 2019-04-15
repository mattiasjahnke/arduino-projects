# morse-keyboard

Turns your Arduino UNO in to a one-button keyboard using morse code.

## How it works

The `loop` function awaits a button press, when detected it measures how long the button was pressed for.

The press duration is converted to either `.` (dot) or `-` (dash). The dot/dash is stored in a buffer. When no button press is received in some time, we'll look up the alphabetical index for the current buffer (combination of `.` and `-`).

The keycode for the letter `A + <index>` is sent to the computer over USB.

## Installation (Mac)

Prerequisite: install dfu-programmer [http://dfu-programmer.github.io/](http://dfu-programmer.github.io/)

1) Connect a button to `digital pin 6` on the Arduino.
2) Upload the sketch `morse-keyboard.ino`.
4) Connect the two pins closest to the USB port on the Arduino using a jumper cable. An LED will blink.
5) In the folder "firmware" execute the shell script `./set-keyboard`.
6) Power cycle the Arduino by disconnecting and connecting the USB cable.

You computer will now recognize the Arduino as a keyboard.

Note: You can't upload a sketch when in "keyboard mode". You can put the Arduino back in serial mode (and allow uploading of sketches) by doing the same procedure but instead running `./set-serial`.

Note: Make sure you're using the right firmware for your Arduino... Firmware fiddling can be tricky ;)


### Special thanks to

https://github.com/nylen/auto-f1-presser
