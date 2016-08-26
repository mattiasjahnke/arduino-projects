// PINS ============================================
#define TRIGGER_PIN     6 // Distance sensor TRIG
#define ECHO_PIN        7 // Distance sensor ECHO
#define PIEZO_PIN       10 // Piezo
#define BUTTON_PIN      2 // Button

// TONES  ==========================================
#define  c     3830    // 261 Hz
#define  d     3400    // 294 Hz
#define  e     3038    // 329 Hz
#define  f     2864    // 349 Hz
#define  g     2550    // 392 Hz
#define  a     2272    // 440 Hz
#define  b     2028    // 493 Hz
#define  C     1912    // 523 Hz

// GLOBAL VARIABLES ================================
const int NOTES[] = {c, d, e, f, g, a, b, C};
const int NECK_LENGTH_CM = 50;
int numberOfNotes = 0; // Populated during setup

void setup() {
  Serial.begin(9600); // Set serial out if we want debugging

  numberOfNotes = sizeof(NOTES) / sizeof(int);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUTTON_PIN, INPUT);

  pinMode(PIEZO_PIN, OUTPUT);
}

void loop() {
  // Wait for button event
  if (digitalRead(BUTTON_PIN) == HIGH) {
    int distance = getDistance();
    distance = constrain(distance, 0, NECK_LENGTH_CM);
    int noteIndex = map(distance, 0, NECK_LENGTH_CM, 0, numberOfNotes - 1);
    Serial.println(noteIndex);
    while (digitalRead(BUTTON_PIN) == HIGH) {
      playSingleTone(NOTES[noteIndex]);
    }
  }
}

void playSingleTone(int tone) {
  if (tone == 0) {
    return;
  }
  digitalWrite(PIEZO_PIN, HIGH);
  delayMicroseconds(tone / 2);
  digitalWrite(PIEZO_PIN, LOW);
  delayMicroseconds(tone / 2);
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
