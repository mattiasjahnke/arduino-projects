/* Written by @engineerish at some point in the year 2018.
 It's not an exact science - do with this code as you please.
 
 Check out more of my projects over at
 https://instagram.com/engineerish */

import processing.serial.*;

Serial arduino;

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

  // Lazy search for the Arduino serial port
  String[] serials = Serial.list();
  for (String s : serials) {
    if (s.contains("/dev/cu.usb")) {
      arduino = new Serial(this, s, 115200);
      break;
    }
  }

  // 0xffff clears the screen (also triggered by tapping 'c' on the keyboard)
  arduino.write(0xffff);
}

void draw() {
  background(0);
  
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
    for (int y = 0; y < matrix.length; y++) {
      for (int x = 0; x < matrix[y].length; x++) {
        matrix[y][x] = false;
      }
    }
    arduino.write(0xffff);
  }
}

// === UTILITIES ===

void sendInteger(int value) {
 // Split the integer up in two that can later be assembeled again on the other side.
 // The data type int in Processing are 32 bit while they're only 16 bit on the Arduino UNO.
  
  int p1 = value & 0xFF;
  int p2 = (value >> 8) & 0xFF;
  arduino.write(p1);
  arduino.write(p2);
}