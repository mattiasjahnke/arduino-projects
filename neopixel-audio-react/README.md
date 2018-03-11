# Neopixels reacting to audio

Have neopixels light up with the rythm of music!

## How it works

### Processing application

An app written in Processing will listen for audio. If the levels reach beyond a given threshold - an event is sent via Serial over to an Arduino.

### Arduino

The Arduino is listening for events sent by the Processing application and will light up a strip of NeoPixels.

## Get started

* Follow [Adafruits guide to power the NeoPixels](https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels)
* Without starting the Processing application, upload the Arduino code on to an Arduino
* Start the Processing application, look at the output in the console to find which index your Arduino have in the list of Serial devices.
* Modify the Serial-constructor to use a suitable index
* Run the Processing application