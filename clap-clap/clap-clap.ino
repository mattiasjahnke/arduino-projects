// === PIN CONFIG ===
int LED_PIN = 2;
int MIC_PIN = 0;

// === SETTINGS ===
int THRESHOLD = 270;
int TIMEOUT = 800;
int PEAKS_TO_TOGGLE = 2;

const int sampleWindow = 50;

// === STATE VARIABLES ===
unsigned long lastPeak = 0;
int peakCounter = 0;
int statePin = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Peak (clap) found?
  unsigned int noise = getNoiseLevel();
  if (noise > THRESHOLD) {
    Serial.println(noise);
    peakCounter++;
    lastPeak = millis();
  }

  // Time out?
  if (millis() - lastPeak > TIMEOUT && peakCounter > 0) {
    peakCounter = 0;
  }

  // Time to toggle?
  if (peakCounter == PEAKS_TO_TOGGLE) {
    toggleLights();
    peakCounter = 0;
  }
}

void toggleLights() {
  statePin = !statePin;
  digitalWrite(LED_PIN, statePin);
}

unsigned int getNoiseLevel() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  unsigned int sample;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MIC_PIN);
    if (sample < 1024) {
      if (sample > signalMax) { // save just the max levels
        signalMax = sample;
      } else if (sample < signalMin) { // save just the min levels
        signalMin = sample;
      }
    }
  }
  
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  double noise = (peakToPeak * 3.3) / 1024;  // convert to volts

  return peakToPeak;
}
