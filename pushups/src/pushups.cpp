#include <Arduino.h>
#include <TM1637Display.h>

// Pin configuration
#define ECHO_PIN 3
#define TRIGGER_PIN 4
#define PIEZO_PIN 11
#define LED1_PIN 5
#define LED2_PIN 6

// Program parameters
#define SLACK 8 // The person has to be +/- SLACK from the position

unsigned long previousMillis = 0; // will store last time the piezo buzzed

long skippedSamples[3];
int skips = 3;
int skippedSampleIndex = 0;
long samples[5];
int nrSamples = 5;
int sampleIndex = 0;

long upDistance = 0;
long downDistance = 0;
bool highTextShown = false;
bool lowTextShown = false;
int calibrationCounter = 0;
int calibrationIterations = 20;
bool waitingForUpDistance = true;

TM1637Display display(12, 13);
int score = 0;

int getDistance();
long getMidDistance(int samples, unsigned long delayMs);

void setup() {
  display.setBrightness(0x0f);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {

  // Calculate global rolling median
  long dist = getDistance();
  if (sampleIndex > 0 && abs(samples[sampleIndex - 1] - dist) > 100) {
    skippedSamples[skippedSampleIndex++] = dist;
    if (skippedSampleIndex >= skips) {
      for (int i = 0; i < skips; i++) {
        samples[i] = skippedSamples[i];
        sampleIndex = skips;
      }
    }
  } else {
    skippedSampleIndex = 0;
    samples[sampleIndex++] = dist;
  }

  long mid = 0;
  for (int i = 0; i < nrSamples; i++) {
    mid += samples[i];
  }
  mid /= nrSamples;

  if (mid < 0) {
    mid = 0;
  }

  if (sampleIndex >= nrSamples) {
    sampleIndex = 0;
  }
  //  -----

  if (upDistance == 0) {
    if (!highTextShown) {
      // Display "SET HI"
      uint8_t setBytes[] = {SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
                            SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
                            SEG_A | SEG_C | SEG_B,
                            0};
      display.setSegments(setBytes, 4, 0);
      delay(1000);

      uint8_t hiBytes[] = {SEG_F | SEG_E | SEG_B | SEG_C | SEG_G, SEG_F | SEG_E, 0, 0};
      display.setSegments(hiBytes, 4, 0);
      delay(1000);

      highTextShown = true;
    }

    if (calibrationCounter >= calibrationIterations) {
      upDistance = mid;
      calibrationCounter = 0;
    } else {
      calibrationCounter++;
    }
  }

  if (upDistance > 0 && downDistance == 0) {
    if (!lowTextShown) {
      // Display "SET LO"
      uint8_t setBytes[] = {SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
                            SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
                            SEG_A | SEG_C | SEG_B,
                            0};
      display.setSegments(setBytes, 4, 0);
      delay(1000);

      uint8_t loBytes[] = {SEG_D | SEG_E | SEG_F, SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, 0, 0};
      display.setSegments(loBytes, 4, 0);
      delay(1000);

      lowTextShown = true;
    }

    if (calibrationCounter >= calibrationIterations) {
      downDistance = mid;
      display.showNumberDec(score);
      calibrationCounter = 0;
    } else {
      calibrationCounter++;
    }
  }

  if (upDistance == 0 || downDistance == 0) {
    display.showNumberDec((int)mid);
  } else {
    if (waitingForUpDistance) {
      if (mid >= upDistance - SLACK && mid <= upDistance + SLACK) {
        waitingForUpDistance = false;
        score++;
        display.showNumberDec(score);
      }
    } else {
      if (mid >= downDistance - SLACK && mid <= downDistance + SLACK) {
        waitingForUpDistance = true;
      }
    }
  }

  delay(20);
}

long getMidDistance(int samps, unsigned long delayMs) {
  long distSamples[samps];

  for (int i = 0; i < samps; i++) {
    distSamples[i] = getDistance();
    delay(delayMs);
  }

  long mid = 0;

  for (int i = 0; i < samps; i++) {
    mid += distSamples[i];
  }

  mid /= samps;
  return mid;
}

int getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance
  return duration * 0.034 / 2;
}
