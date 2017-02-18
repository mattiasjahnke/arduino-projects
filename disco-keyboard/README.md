# disco-keyboard

This project build can turn you keyboard into a sparkling light show using Arduino and neopixels.

## Circuit

Cut a neopixel strip into a strip with the same number of pixels as you have keys on a row on your keyboard.
Find a number that kind of works for every row.

Connect neopixels to the Arduino pin 9. Read more about neopixels and how to connect them here [https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections](https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections).

## Prepare the code

Modify the keyboard array to represent your own keyboard.
```
int keyboard[4][14] = {...}
```

If you wan't to change the color for each row, modify the `colors` array.

## How it works

Your computer will, whenever the user taps a key on the keyboard, send a number via Serial to the Arduino.
This number is converted on the Arduino into a color and an index (to match the location of the key) that will represent a neopixel.

## The Processing

This project ships with an implementation of the "keylogger" in Processing (read more on [https://processing.org/](https://processing.org/)). Processing is platform independent and very simple - making it a suitable choice for this open source demo.

This comes with the limitation that the user will have to have the "Processing" window in focus in order to send key strokes to the Arduino. I suggest you write your own implementation to work "system wide".
