#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 5;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 20; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

const unsigned char myBitmap [] PROGMEM = {
  0xe7, 0xe7, 0xe7, 0x18, 0x18, 0xe7, 0xe7, 0xe7
};

void setup() {
  Serial.begin(9600);
  matrix.setIntensity(0); // Use a value between 0 and 15 for brightness
  
}

void loop() {
  drawFireworks();

  matrix.setIntensity(15);
  loopTape("Happy New Year! :)");

  matrix.fillScreen(LOW);

  matrix.setIntensity(0);
  drawString("2018");
  for (int i = 0; i < 16; i++) {
    matrix.setIntensity(i);
    delay(100);
  }

  delay(1000);
  
  matrix.fillScreen(LOW);
  matrix.setIntensity(15);

  loopTape("@engineerish");
}

void drawFireworks() {
  int x = matrix.width() / 2 - 1;
  
  for (int y = 7; y > 4; y--) {
    matrix.fillScreen(LOW);
    matrix.drawPixel(x, y, HIGH);
    matrix.write();
    delay(100);
  }

  int r = 2;
  for (int i = 0; i < 20; i++) {
    matrix.setIntensity(min(i, 15));
    matrix.fillScreen(LOW);
    int r0 = r;
    while (r0 >= 2) {
      matrix.drawCircle(x, 4, r0, HIGH);
      r0 -= 6;
    }
    matrix.write();
    delay(100);
    r += 1;
  }
}

void drawString(String string) {
  int startingX = matrix.width() / 2 - string.length() * width / 2;
  int y = (matrix.height() - 8) / 2; // center the text vertically
  for (int i = 0; i < string.length(); i++) {
    matrix.drawChar(startingX + width*i, y, string[i], HIGH, LOW, 1);
  }
  matrix.write();
}

void loopTape(String tape) {
  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}

