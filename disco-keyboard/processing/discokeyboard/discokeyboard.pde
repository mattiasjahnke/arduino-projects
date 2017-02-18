import processing.serial.*; 

Serial port;

void setup() {
  size(640, 480);
  // Change the index chosen in the Serial.list() array if needed
  port = new Serial(this, Serial.list()[1], 9600);
}

void draw() { }

void keyPressed() {
  char c = Character.toLowerCase(key);
  port.write(c);
  // When debugging or "building" your keyboard - printing the value is useful.
  //println((int)c);
}