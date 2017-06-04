#include <Servo.h>

#define ECHO_PIN 2
#define TRIGGER_PIN 3

Servo myservo;

int minAngle = 5;
int maxAngle = 175;

long getDistance();
long getMidDistance(int samples, unsigned long delayMs);
void report(int angle, long distance);

void setup() {
  Serial.begin(9600);
  myservo.attach(4);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  for (int angle = minAngle; angle <= maxAngle; angle += 1) {
    myservo.write(angle);
    delay(15);
    report(angle, getDistance());
  }
  for (int angle = maxAngle; angle >= minAngle; angle -= 1) {
    myservo.write(angle);
    delay(15);
    report(angle, getDistance());
  }
}

void report(int angle, long distance) {
  Serial.print(angle);
  Serial.print("+");
  Serial.print(distance);
  Serial.println("");
}

long getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance
  return duration * 0.034 / 2;
}

