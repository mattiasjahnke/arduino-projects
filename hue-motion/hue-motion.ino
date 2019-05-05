#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define PIR_PIN D3

unsigned long idlePeriod = 1000 * 60 * 5;
unsigned long lastMovement = 0;
unsigned long lastTransmission = 0;

int last = LOW;

const char* ssid = "YOUR WIFI SSID";
const char* password = "YOUR WIFI PASSWORD";
const char* endpoint = "http://<BRIDGE IP>/api/<YOUR USER ID>/lights/<LIGHT NUMBER>/state";

void setup(void) {
  pinMode(PIR_PIN, INPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");

  lastMovement = millis();
  lastTransmission = millis();
}
void loop() {
  bool movementDetected = digitalRead(PIR_PIN) == HIGH;
  bool okToSend = last != movementDetected;
  last = movementDetected;

  if (movementDetected) {
    lastMovement = millis();
    Serial.println("Movement detected");
    if (okToSend) {
      Serial.println("Sending ON");
      sendHttpRequest(true);
    }
  } else {
    Serial.println("No movement detected");
    if (millis() - lastMovement > idlePeriod && okToSend) {
      Serial.println("Passed idlePeriod - sending OFF");
      sendHttpRequest(false);
    }
  }
  
  delay(2000);
}

void sendHttpRequest(bool turnOn) {
  lastTransmission = millis();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(endpoint);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (turnOn) {
      http.PUT("{\"on\":true}");
    } else {
      http.PUT("{\"on\":false}");
    }
    http.end();
  } else {
    Serial.println("Request not sent: Not connected");
  }
}
