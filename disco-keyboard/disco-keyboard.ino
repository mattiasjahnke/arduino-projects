#include <Adafruit_NeoPixel.h>
#define PIN            9
#define NUMPIXELS      17

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Change the values to represent you own keyboard
int keyboard[4][14] = {
  {167, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 43, 180, 8},
  {9, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 229, 168, 10},
  {9, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ö', 228, 39, 10},
  {9, 60, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 9, 10},
};

uint32_t colors[] = {
  pixels.Color(200, 0, 200),
  pixels.Color(0, 200, 0),
  pixels.Color(0, 0, 200),
  pixels.Color(200, 0, 0)
};

void setup() {
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  pixels.clear();
  pixels.show();
  
  int r = Serial.read();

  // Handle space slightly different
  if (r == 32) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(100, 100, 100));
    }
    pixels.show();
    delay(20);
    return;
  }

  bool found = false;
  for (int i = 0; i < 4 && !found; i++) {
    for (int j = 0; j < 14 && !found; j++) {
      if (r == keyboard[i][j]) {
        pixels.setPixelColor(j, colors[i]);
        pixels.show();
        found = true;
      }
    }
  }
  delay(20);
}
