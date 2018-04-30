# Matrix Painter

## 📄 Project Description

The "Matrix Painter" is a project where you control 8x8 LED Matrices from either your computer, a friends computer, or any other device such as a phone.

### Arduino

The Arduino listens on the Serial ports for commands. The commands are interpreted and used to control the matrices.

#### Hardware connections

```
VCC -> 5v
GND -> GND
DIN -> Digital pin 11
CS -> Digital pin 10
CLK -> Digital pin 13
```
### Processing

An app written in Processing (read more about Processing [here](https://processing.org/)) connects to the Arduino via the Serial port.

*Note to Windows users: This project was developed on a Mac - you'll need to poke around in the serial port string in order to find the right one*

The app has a UI for controlling the pixels on the matrices.

The app also opens a TCP server on port 5204 which allows for further ways of controlling the matrices by connecting clients.

### iOS

In the project there's a simple iOS app that renders a grid, representing the matrices. The app connects to the TCP server running in the Processing app.

*In order to compile the iOS project you'll need Xcode.*

## 🤓 Code Deep Dive

### The "Processing -> Arduino" protocol

The protocol aims to be as simple and lightweight as possible.

What we want to be able to do is:
* Turn any given LED *on* or *off*
* Clear the entire matrix

We're sending bytes over Serial - and in order for us to address all LEDs in my version of the project we need to be able to communicate at least 320 (8*8*5) different indices/integers which implies two bytes - hence we decide that any command is two bytes long.

*Instead of sending a point as (X, Y) - I flatten the point in to an index to only work with non-complex types.*

```
/* How to convert point to index */
int index = y * width + x;

/* How to convert index to point */
int y = floor(index / width);
int x = index % width;
```

#### Command: Turn any given LED *on* or *off*

In order to communicate the state of a LED to the Arduino, we have to specify both the LED coordinate we aim to control and the desired state (on/off).

First off, with the formula above, we convert the coordinate to a simple index.

Next step is to signify the desired state. One idea could for example be that we send the index as a negative value to turn if off, and positive for on. This approach is simple but opens up for some other problems:

* We have to care about how negative numbers are represented in memory.
* The index "0" becomes a bit problematic (we could of course increment the index by one before adding the sign - but this just makes everything even more complex...)

The solution I decided to go with was to increment the index by 1000 if I wanted to say "turn on".

This means that if I send an integer that is greater of equal to 1000, the Arduino knows that I aim to turn an LED on, and less than 1000 means off.

```
/* (X, Y) */
Serial.print(1000); // Turn led at (0,0) on
Serial.print(1001); // Turn led at (1,0) on
Serial.print(0); // Turn led at (0, 0) off
Serial.print(5); // Turn led at (5, 0) off
```

#### Command: Clear the entire matrix

Sloppy implementation - just send two bytes: `0xff, 0xff`

#### Interpret the commands received

Whenever we detect some bytes available on the Serial stream:
```
while (bytesRecieved < 2) {
  bytes[bytesRecieved++] = Serial.read();
}
```
We make sure to gather the full command (two bytes).

Check if it's a "clear command":

`if (bytes[0] == 0xff && bytes[1] == 0xff) { ... }`

Otherwize we figure it's an index we've recieved. Two bytes are used to represent the integer, we we do some math to combine the two bytes:

`unsigned int val = bytes[1] * 256 + bytes[0];`

And check if we're to turn the LED on or off:

```
int state = val >= 1000 ? HIGH : LOW;
if (val >= 1000) {
  val -= 1000;
}
```

### The "TCP -> Processing" protocol

This is basically just a matter of passing on commands received on the TCP port to the Serial. But we're working with strings instead where each command is separated by a comma ",".

#### Command: Turn any given LED *on* or *off*

We take the string received on the TCP port, turn it in to an integer, check if we're turning the led on or off, update the local matrix (to make the changes happen in the Processing UI) and pass the integer on to the Arduino.

```
int index = Integer.parseInt(part.trim());
boolean add = false;
int mod = index;
if (index >= 1000) {
  add = true;
  mod -= 1000;
}
int y = floor(mod / matrix[0].length);
int x = mod % matrix[0].length;
matrix[y][x] = add;
sendInteger(index);
```

*The function `sendInteger()` will split an integer in to two bytes to not lose all bytes after the first*

```
void sendInteger(int value) {
  int p1 = value & 0xFF;
  int p2 = (value >> 8) & 0xFF;
  arduino.write(p1);
  arduino.write(p2);
}
```

#### Command: Clear the entire matrix

Simply sending the string `"clear"`.

## 📺 YouTube Tutorial

Watch the video on YouTube [here](https://youtu.be/zjX-ri_5tDQ)
