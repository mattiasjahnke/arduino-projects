#include <Adafruit_NeoPixel.h>


#define PIN            6
#define PIXELS_PER_SEGMENT	3
#define DIGITS 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * DIGITS, PIN, NEO_GRB + NEO_KHZ800);

byte segments[10] = {
  0b1111110,
  0b0011000,
  0b0110111,
  0b0111101,
  0b1011001,
  0b1101101,
  0b1101111,
  0b0111000,
  0b1111111,
  0b1111001
};

void writeDigit(int index, int value) {
  byte seg = segments[value];
  for (int i = 6; i >= 0; i--) {
    int offset = index * (PIXELS_PER_SEGMENT * 7) + i * PIXELS_PER_SEGMENT;
    uint32_t color = seg & 0x01 != 0 ? strip.Color(25, 50, 50) : strip.Color(0, 0, 0);
    for (int x = offset; x < offset + PIXELS_PER_SEGMENT; x++) {
      strip.setPixelColor(x, color);
    }
    seg = seg >> 1;
  }
}

void setup() {
  strip.begin();
}

int c = 0;
int b = 0;

void loop() {
  clearDisplay();

  animateLoop(true, true);
  animateLoop(false, false);

  for (int i = 0; i < 10; i++) {
    writeDigit(0, i);
    strip.show();
    delay(1000);
  }

  strip.show();
  delay(1000);
}

void writeClock(int hour, int minute) {
  int num = hour * 100 + minute;
  for (int i = DIGITS - 1; i >= 0; i--) {
    writeDigit(i, num % 10);
    num /= 10;
  }
}

void clearDisplay() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

void animateLoop(bool fromInner, bool clearing) {

  for (int b = fromInner ? 0 : 7; fromInner ? b < 7 : b >= 0; b += (fromInner ? 1 : -1)) {
    if (clearing) {
      clearDisplay();
    }
    switch (b) {
      case 0:
        strip.setPixelColor(19, strip.Color(100, 0, 100));
        break;
      case 1:
        strip.setPixelColor(20, strip.Color(100, 0, 100));
        strip.setPixelColor(18, strip.Color(100, 0, 100));
        break;
      case 2:
        strip.setPixelColor(0, strip.Color(100, 0, 100));
        strip.setPixelColor(8, strip.Color(100, 0, 100));
        strip.setPixelColor(9, strip.Color(100, 0, 100));
        strip.setPixelColor(17, strip.Color(100, 0, 100));
        break;
      case 3:
        strip.setPixelColor(1, strip.Color(100, 0, 100));
        strip.setPixelColor(7, strip.Color(100, 0, 100));
        strip.setPixelColor(10, strip.Color(100, 0, 100));
        strip.setPixelColor(16, strip.Color(100, 0, 100));
        break;
      case 4:
        strip.setPixelColor(2, strip.Color(100, 0, 100));
        strip.setPixelColor(6, strip.Color(100, 0, 100));
        strip.setPixelColor(11, strip.Color(100, 0, 100));
        strip.setPixelColor(15, strip.Color(100, 0, 100));
        break;
      case 5:
        strip.setPixelColor(3, strip.Color(100, 0, 100));
        strip.setPixelColor(5, strip.Color(100, 0, 100));
        strip.setPixelColor(12, strip.Color(100, 0, 100));
        strip.setPixelColor(14, strip.Color(100, 0, 100));
        break;
      case 6:
        strip.setPixelColor(4, strip.Color(100, 0, 100));
        strip.setPixelColor(13, strip.Color(100, 0, 100));
        break;
      default: break;
    }
    strip.show();
    delay(100);
  }
}

