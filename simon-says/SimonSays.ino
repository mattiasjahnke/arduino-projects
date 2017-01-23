// LED & Button Configuration
// The index of a LED must correlate to that of button of the same color.
// If leds[0] == redPin then buttons[0] must be redButtonPin
int leds[]    = {2, 4, 6, 8};
int buttons[] = {3, 5, 7, 9};

// Game logic and settings
int simonLedOnInterval = 1000; // How long Simons LED is on for before going to next
int simonMemory[100]; // This is where simons sequence is stored (pre-calculated for each game)

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    for (int i = 0; i < 4; i++) {
        pinMode(leds[i], OUTPUT);
        pinMode(buttons[i], INPUT);
    }   
}

void loop() {
    Serial.println("New game is starting...");

    // Generate Simons complete sequence
    for (int i = 0; i < 100; i++) {
        simonMemory[i] = random(0, 4);
    }
    
    playStartAnimation();
    
    bool failed = false;
    int currentMemory = 0;
    delay(1000);

    Serial.println("New game is started");
    
    while (!failed) {
        playSequenceStartingAnimation();
        delay(200);

        // Playback simons memory up to currentMemory
        playSimonsSequenceToIndex(currentMemory);

        // Let the player try input the sequence
        Serial.println("Waiting for input");
        bool sequenceComplete = false;
        int sequenceIndex = 0;
        
        while (!sequenceComplete) {
            int buttonPressedPin = nextButtonPressed();
            if (buttonPressedPin != buttons[simonMemory[sequenceIndex]]) { // Wrong button pressed
                Serial.println("Player failed to follow Simon!");
                failed = true;
                break;
            }

            if (sequenceIndex == currentMemory) {
                Serial.println("Player successfully followed Simon!");
                sequenceComplete = true;
            }
            sequenceIndex++;
        }
        
        delay(200);
        currentMemory++;
    }

    delay(500);
}

void playSimonsSequenceToIndex(int index) {
    Serial.println("Playing simons memory sequence...");
        for (int i = 0; i <= index; i++) {
            if (i > 0) {
                digitalWrite(leds[simonMemory[i - 1]], LOW); // Turn off last LED
                delay(250);
            }
            digitalWrite(leds[simonMemory[i]], HIGH);
            delay(simonLedOnInterval);
        }
        digitalWrite(leds[simonMemory[index]], LOW);
}

int nextButtonPressed() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            int buttonPin = buttons[i];
            int correspondingLed = leds[i];
            if (digitalRead(buttonPin) == HIGH) {
                digitalWrite(correspondingLed, HIGH);
                while (true) {
                    if (digitalRead(buttonPin) == LOW) {
                        digitalWrite(correspondingLed, LOW);
                        return buttonPin;
                    }
                    delay(50);
                }
            }
        }
    }
}

void playStartAnimation() {
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                digitalWrite(leds[3], LOW);
            } else {
                digitalWrite(leds[i - 1], LOW);
            }
            digitalWrite(leds[i], HIGH);
            delay(100);
        }
    }
    digitalWrite(leds[3], LOW);
}

void playSequenceStartingAnimation() {
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 4; i++) { digitalWrite(leds[i], HIGH); }
        delay(100);
        for (int i = 0; i < 4; i++) { digitalWrite(leds[i], LOW); }
        delay(100);
    }
}

