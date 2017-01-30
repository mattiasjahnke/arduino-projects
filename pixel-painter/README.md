# Pixel-Painter

## What is it

The idea is that you prep your Arduino with an image. This image will be rendered one column at a time on your neopixel strip. You'll setup a camera (or iPhone camera with some fancy app) to have a really long exposure.
When you move the pixel strip horizontally, as the camera image is being exposed, you should get your original image as result. Somewhat blurry maybe (and low resolution).

## Circuit
Not much to it; connect your neopixel strip to the Arduino. Attach that neopixel strip to something. Like a plank.

## Code

This project consists of two parts

### Arduino code

The code that will turn your neopixel strip into a blur of colors.

(There's more detailed instructions inside the source code)

### Processing code

The application that will turn your image into something useful for the Arduino is written in Processing.

If you've never heard of processing you can read all about it [here](https://processing.org/).