/*

 Written by @engineerish at some point in the year 2018.
 It's not an exact science - do with this code as you please.

 Check out more of my projects over at
 https://instagram.com/engineerish


 Project: Led Matrix Painter
 Description:
 This Processing app provides you with a GUI for controlling the 8x8 led matrices connected to an Arduino.
 The app communicates over Serial (baud rate 115200)
 There's a TCP server running (port 5204 by default), so you can control the matrix through any other app if you'd like
 and only use this Processing sketch as a proxy to speak to the Arduino.

 Protocol: This sketch will send an indices of the led to edit. In order to turn on a led, you'd send the
 index of the led + 1000 as an integer over to the Arduino (1000 will for example turn on the first led)

 To turn off a led, you'd send an integer < 1000 (sending 0 turns off the first led)

 To test out the socket connection in terminal:

 echo 1000 | nc localhost 5204

 */


import processing.serial.*;
import processing.net.*;

Serial arduino;
Server server;

static int cellSize = 32;

// In my setup I used 5 displays organized horizontally.
// Modify this matrix and the Arduino code accordingly to your setup.

boolean[][] matrix = new boolean[8][40];
int lastCellX = -1;
int lastCellY = -1;
boolean additive = true;

void setup() {
  // Matrix is 1280x257
  size(1320, 297);
  noStroke();

  tryConnectSerial();

  // Setup TCP server
  server = new Server(this, 5204);
}

void tryConnectSerial() {
  // Lazy search for the Arduino serial port
  String[] serials = Serial.list();
  for (String s : serials) {
    if (s.contains("/dev/cu.usb")) {
      arduino = new Serial(this, s, 115200);

      clearMatrix();
      break;
    }
  }
}

void draw() {
  background(0);

  if (arduino == null) {
    textSize(32);
    fill(255, 0, 0);
    text("No arduino connected!", 500, 120);
    fill(255);
    text("Waiting for connection...", 490, 160);

    tryConnectSerial();

    delay(300);
    return;
  }

  Client client = server.available();
  if (client != null) {
    String clientMessage = client.readString();
    if (clientMessage != null) {
      String[] parts = clientMessage.split(",");
      for (String part : parts) {
        String trimmed = part.trim();
        if (trimmed.equals("clear")) {
          clearMatrix();
        } else if (!trimmed.equals("")) {
          int index = Integer.parseInt(trimmed);
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
        }
      }
    }
  }

  // Draw Matrix
  for (int y = 0; y < matrix.length; y++) {
    for (int x = 0; x < matrix[y].length; x++) {
      fill(matrix[y][x] ? #f4c842 : #282828);
      ellipse(20 + x * cellSize + cellSize / 2, 20 + y * cellSize + cellSize / 2, cellSize, cellSize);
    }
  }
}

// === MOUSE HANDLING ===
void mousePressed() {
  int cellX = ceil((mouseX - 20) / cellSize);
  int cellY = ceil((mouseY - 20) / cellSize);

  if (cellY < 0 || cellX < 0 || cellY >= matrix.length || cellX >= matrix[0].length) {
    return;
  }

  // If the user begins the mouse interaction on an "unlit" cell - the click and continous drag will light up leds (additive)
  additive = !matrix[cellY][cellX];

  handleMouse();
}

void mouseDragged() {
  handleMouse();
}

void mouseReleased() {
  lastCellX = -1;
  lastCellY = -1;
}

void handleMouse() {
  int cellX = ceil((mouseX - 20) / cellSize);
  int cellY = ceil((mouseY - 20) / cellSize);

  if (cellY < 0 || cellX < 0 || cellY >= matrix.length || cellX >= matrix[0].length) {
    return;
  }

  // Make sure we don't change the same cell over and over again when dragging over the same cell
  if (lastCellX != cellX || lastCellY != cellY) {
    lastCellX = cellX;
    lastCellY = cellY;

    // Modify the local matrix to reflect this change during next render pass
    matrix[cellY][cellX] = additive;

    // Calculate the index of the led to modify (counted from top left)
    int index = (cellY * matrix[0].length + cellX);

    // "Adds" to the matrix are represented by a led index > 1000 (this value is later subtracted on the Arduino)
    if (matrix[cellY][cellX]) {
      index += 1000;
    }

    // Send this integer (index of the cell to change) to the Arduino over Serial
    sendInteger(index);
  }
}

// === KEYBOARD HANDLING ===

void keyReleased() {
  if (key == 'c') {
    clearMatrix();
  }
}

void clearMatrix() {
  for (int y = 0; y < matrix.length; y++) {
    for (int x = 0; x < matrix[y].length; x++) {
      matrix[y][x] = false;
    }
  }
  arduino.write(0xffff);
}

// === UTILITIES ===

void sendInteger(int value) {
  int p1 = value & 0xFF;
  int p2 = (value >> 8) & 0xFF;
  arduino.write(p1);
  arduino.write(p2);
}
