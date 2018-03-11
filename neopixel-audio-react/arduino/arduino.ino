#include <Adafruit_NeoPixel.h>

#define PIN 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(290, PIN, NEO_GRB + NEO_KHZ800);
int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    float alpha = incomingByte / 100.0f;
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255 * alpha, 0 * alpha, 255 * alpha));
    }
  } else {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  strip.show();
}
