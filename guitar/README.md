# arduino-guitar

Source code for a guitar built with an Arduino.

Components used:
* Arduino Uno
* Momentary button
* Distance sensor
* Piezo
* Some resistors and wires

## How it works

When the "musician" pushes the button, we'll read the distance from the sensor to the object closest to it.
Using the distance, we'll then map that value onto a list of notes and output that note on the piezo.
