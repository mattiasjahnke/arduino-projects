// Written by Mattias Jähnke
// Some time in January 2017
// It's not an exact science.
// Do with this code as you please
// https://instagram.com/engineerish

// Instructions:
// * Place your image inside of the "data" folder
// * Change the height-value when setting size(); to the number of neopixels you have
// * Change the image name in loadImage();
// * Run and follow the instructions in the arduino code

PImage img;

void setup() {
  size(200, 60); // <-- Set y (60 is my case) to how many neopixels you have
  img = loadImage("arduino.png"); // <-- This is a line of code you can change
  img.resize(0, height);
  print("Image size width: ");
  print(img.width);
  print(", height: ");
  println(img.height);
  println("");
}

void draw() {
  img.loadPixels();
  print("const long image[][");
  print(height);
  print("] PROGMEM = {\n");
  for (int x = 0; x < img.width; x++) {
    print("{ ");
    for (int y = 0; y < img.height; y++) {
      int loc = x + y*img.width;
      if (loc >= img.pixels.length) {
        break;
      }
      stroke(img.pixels[loc]);
      print("0x" + hex(img.pixels[loc], 6));
      if (y < img.height - 1) {
        print(", ");
      }
      point(width / 2 - img.width / 2 + x, y);
    }
    print("}");
    if (x < img.width - 1) {
      print(", ");
    }
    print("\n");
  }
  print("};");
  noLoop();
}