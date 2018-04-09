/* Written by @engineerish at some point in the year 2018.
 It's not an exact science - do with this code as you please.
 
 Check out more of my projects over at
 https://instagram.com/engineerish */


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 5;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void setup() {
  Serial.begin(115200);
  matrix.setIntensity(15);
  matrix.fillScreen(LOW);
  matrix.write();
}

int bytesRecieved = 0;
uint8_t bytes[2];

void loop() {
  if (Serial.available()) {
    while (bytesRecieved < 2) {
      bytes[bytesRecieved++] = Serial.read();
    }

    bytesRecieved = 0;

    if (bytes[0] == 0xff && bytes[1] == 0xff) {
      matrix.fillScreen(LOW);
      matrix.write();
      bytes[0] = 0x00;
      bytes[1] = 0x00;
      return;
    }

    unsigned int val = bytes[1] * 256 + bytes[0];

    int state = val >= 1000 ? HIGH : LOW;
    if (val >= 1000) {
      val -= 1000;
    }

    int y = floor(val / (numberOfHorizontalDisplays * 8));
    int x = val % (numberOfHorizontalDisplays * 8);
    matrix.drawPixel(x, y, state);
    matrix.write();
  }

  delay(10);
}

