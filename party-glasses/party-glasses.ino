#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEFT_PIN       3
#define RIGHT_PIN      5
#define NUMPIXELS      16

Adafruit_NeoPixel leftPixels = Adafruit_NeoPixel(NUMPIXELS, LEFT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightPixels = Adafruit_NeoPixel(NUMPIXELS, RIGHT_PIN, NEO_GRB + NEO_KHZ800);

int animation = 0;
int animationLoops = 0;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  leftPixels.begin();
  rightPixels.begin();
}

void loop() {
  if (animationLoops == 4) {
    animationLoops = 0;
    animation += 1;
    if (animation == 2) {
      animation = 0;
    }
  }
  if (animation == 0) {
    for (int i = 0; i < NUMPIXELS; i++) {
      leftPixels.setPixelColor(i, leftPixels.Color(13 + i * 12,   0,  13 + i * 12));
      rightPixels.setPixelColor(NUMPIXELS - 1 - i, rightPixels.Color(13 + i * 12, 0,  13 + i * 12));
      leftPixels.show();
      rightPixels.show();

      delay(50); // Delay for a period of time (in milliseconds).
    }

    for (int i = 0; i < NUMPIXELS; i++) {
      leftPixels.setPixelColor(i, leftPixels.Color(0, 0, 0));
      rightPixels.setPixelColor(NUMPIXELS - 1 - i, rightPixels.Color(0, 0, 0));
      leftPixels.show();
      rightPixels.show();

      delay(50); // Delay for a period of time (in milliseconds).
    }
  } else if (animation == 1) {
    for (int k = 0; k < 4; k++) {
      for (int i = 0; i < NUMPIXELS; i++) {
        leftPixels.setPixelColor(i, leftPixels.Color(255, 0, 0));
        rightPixels.setPixelColor(i, rightPixels.Color(0, 0, 0));
        leftPixels.show();
        rightPixels.show();
      }

      delay(100); // Delay for a period of time (in milliseconds).

      for (int i = 0; i < NUMPIXELS; i++) {
        leftPixels.setPixelColor(i, leftPixels.Color(0, 0, 0));
        rightPixels.setPixelColor(i, rightPixels.Color(0, 0, 255));
        leftPixels.show();
        rightPixels.show();
      }

      delay(100); // Delay for a period of time (in milliseconds).
    }
  }
  animationLoops += 1;
}
