#include <Arduino.h>

#define PHOTOCELL_PIN 0
int reading;

void setup() {
  Serial.begin(9600);
  Serial.println("ok");
}

void loop() {
  reading = analogRead(PHOTOCELL_PIN);
  Serial.print("Analog reading = ");
  Serial.println(reading);

  delay(100);
}
