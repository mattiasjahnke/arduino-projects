#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <RTCZero.h>
#include "DFRobotDFPlayerMini.h"

#include "arduino_secrets.h"
#include "clockdef.h"

#define BUTTON_PIN 2
#define GREEN_PIN 4
#define RED_PIN 3
#define BUSY_PIN 5


DFRobotDFPlayerMini myDFPlayer;

RTCZero rtc;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

const int GMT = 2; //change this to adapt it to your time zone

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(100);
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BUSY_PIN, INPUT);

  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(RED_PIN, LOW);

  SetupMP3();
  setupWifi();
  setupClock();

  digitalWrite(GREEN_PIN, HIGH);

  Serial.println("==== ALL GOOD TO GO!");
}

void loop() {
  // If the phone is picked up
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button Pressed");
    sayTime(rtc.getHours() + GMT, rtc.getMinutes());
    delay(1000);
  }

  delay(10);
}

void setupWifi() {
  Serial.println("==== Setting up wifi...");

  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    digitalWrite(GREEN_PIN, LOW);
    Serial.println("WiFi shield not present");
    digitalWrite(RED_PIN, HIGH);
    while (true) {
      delay(1000);
    }
  }

  int attempt = 0;

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
    if (++attempt == 4) {
      digitalWrite(RED_PIN, HIGH);
      while (true) {
        delay(1000);
      }
    }
  }

  digitalWrite(GREEN_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_PIN, LOW);

  printWiFiStatus();
}

void SetupMP3() {
  Serial.print("MP3:Init");
  Serial1.begin(9600);
  while (!Serial1)
    Serial.print(".");
  Serial.print("OK ");

  if (!myDFPlayer.begin(Serial1)) {
    int count = 10;
    while (count--) {
      Serial.print(".");
      delay(500);
    }
  }
  Serial.print("OK ");
  Serial.println(myDFPlayer.readState());

  Serial.println("MP3:Config");
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
  myDFPlayer.outputSetting(true, 31);
  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);

  //----Set device we use SD as default----
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);

  //----Mp3 control----
  //  myDFPlayer.sleep();     //sleep
  //  myDFPlayer.reset();     //Reset the module
  //  myDFPlayer.enableDAC();  //Enable On-chip DAC
  //  myDFPlayer.disableDAC();  //Disable On-chip DAC
  //  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15

  //----Read imformation----
  Serial.print("MP3:readState:");
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.print("MP3:readVolume:");
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.print("MP3:readEQ:");
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.print("MP3:readFC:");
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.print("MP3:readCFN:");
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.print("MP3:readFCIF:");
  Serial.println(myDFPlayer.readFileCountsInFolder(1)); //read fill counts in folder SD:/03
}

void syncPlay(int fileIndex) {
  Serial.print("Play: ");
  Serial.println(fileIndex);
  myDFPlayer.play(fileIndex);
  delay(100);
  int playerState = 0;
  while(playerState != 512) {
    delay(400);
    playerState = myDFPlayer.readState();
  }
}

void sayTime(int h, int m) {
  h = h % 12;
  syncPlay(fTimeIs);

  if (m) {
    for (int i = 0; minutes[m][i]; i++) {
      syncPlay(minutes[m][i]);
    }
    syncPlay(fMinutes);
    syncPlay(fPast);
  }
  syncPlay(hours[h]);
}



void setupClock() {
  Serial.println("==== Setting up real time clock...");
  rtc.begin();

  unsigned long epoch;
  int numberOfTries = 0, maxTries = 16;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
    delay(100);
  } while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {
    Serial.print("NTP unreachable!!");
    digitalWrite(RED_PIN, HIGH);
    while (true) {
      delay(1000);
    }
  } else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);

    Serial.println();
  }

  digitalWrite(GREEN_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_PIN, LOW);
}


void printTime() {
  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());
  Serial.println();
}

void printDate() {
  Serial.print(rtc.getDay());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear());

  Serial.print(" ");
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}
