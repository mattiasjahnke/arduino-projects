#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define NEXT_PIN 6
#define TOGGLE_PIN 7
#define PREVIOUS_PIN 8

String message;
double previous = 0;
int pos = 0;
int delta = 1;
int previousVolume = -1;

void setup() {

  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(PREVIOUS_PIN, INPUT_PULLUP);
  
  lcd.begin(16, 2);

  analogWrite(A0, 127);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    message = Serial.readString();
    pos = 0;
    lcd.clear();
    for (int i = 0; i < message.length(); i++) {
      if (message.charAt(i) != '\n') {
        lcd.write(message.charAt(i));
      } else {
        lcd.setCursor(0, 1);
      }
    }
  }

  if (digitalRead(NEXT_PIN) == LOW) {
    Serial.write("N");
    delay(500);
  } else if (digitalRead(PREVIOUS_PIN) == LOW) {
    Serial.write("P");
    delay(500);
  } else if (digitalRead(TOGGLE_PIN) == LOW) {
    Serial.write("T");
    delay(500);
  }

  int volume = map(analogRead(A1), 0, 1023, 0, 100);
  if (abs(volume - previousVolume) > 5) {
    if (previousVolume > 0) {
      Serial.write("V");
      Serial.write(volume);
    }
    previousVolume = volume;
    delay(200);
  }

  //printLine(400);
}

void printLine(double refresh) {
  if (millis() - previous > refresh) {
    if (delta > 0 && pos < message.length()) {
      lcd.scrollDisplayLeft();
      pos++;
    }
  }
}
