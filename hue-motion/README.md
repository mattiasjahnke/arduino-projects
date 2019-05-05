# hue-motion

Detecting motion with a PIR sensor, a NodeMCU communicates with a Philips Hue bridge to toggle the lights on or off (like the philips motions sensor).

## Get started

The circuit is simple (connect the PIR sensor to the NodeMCU). Tricky thing with the NodeMCU is that it outputs 3.3v while the PIR sensor runs on 5v. You can override the voltage regulator on the PIR by soldering a wire (https://www.instructables.com/id/Convert-a-5v-PIR-Motion-Sensor-to-33v-for-ESP8266/).

Modify the sketch with your wifi ssid and password.

Setup a user id on your Philips Hue bridge (see the developer documentation) and modify the hard coded URL in the sketch to target your specific light.
