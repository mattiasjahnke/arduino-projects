# Radar

## What is it

A simple build that will plot the distance from the servo to an object in a sweeping motion, using Processing.

## Circuit

Attach an ultrasonic sensor to a servo, hook them up to an Arduino board.

## Code

This project consists of two parts

### Arduino code

Sweeps the servo back and forth. After every step, it will read the distance off the ultrasonic sensor and write the value to Serial.


### Processing code

Listens for "readings" (off the Serial port) from the Arduino and plots the result with simple graphics.

You can read more about Processing [here](https://processing.org/).
