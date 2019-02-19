#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define WIFI_SSID "WiFi ID Here"
#define WIFI_PASS "Password"
#define SERIAL_BAUDRATE 115200

const int Phase1 = D5;
const int Phase2 = D6;
const int Phase3 = D7;
const int Phase4 = D8;

const int SetClose = D0;
const int RunClose = D1;

int stepCount = 0;
int OpeningPosition = 0;
int ClosingPosition = 0;
int OpenBlinds = 0;
int CloseBlinds = 0;

fauxmoESP fauxmo;
// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);
  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());



}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Phase1, OUTPUT);
  pinMode(Phase2, OUTPUT);
  pinMode(Phase3, OUTPUT);
  pinMode(Phase4, OUTPUT);
  pinMode(SetClose, INPUT);
  pinMode(RunClose, INPUT);
  OpenBlinds = 0;
  CloseBlinds = 0;
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("FauxMo demo sketch");
  Serial.println("After connection, ask Alexa/Echo to '<devicename> open', 'close'");
  // Wifi
  wifiSetup();
  // Fauxmo
  fauxmo.addDevice("Blinds");
  // Gen3 Devices or above
  fauxmo.setPort(80);
  // Allow the FauxMo to be discovered
  fauxmo.enable(true);
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.print("Device: "); Serial.print(device_name);
    Serial.print(" state");
    if (state) {
      Serial.println("Open");
      OpenBlinds = 1;
      CloseBlinds = 0;

    }
    else {
      Serial.println("Close");
      OpenBlinds = 0;
      CloseBlinds = 1;

    }
  });

}

void loop() {
  fauxmo.handle();
  //Signal from Alexa to Open


  if (OpenBlinds == 1 && (stepCount <= 0)) {
    OpeningPosition = stepCount + 512;
  }
  if (OpenBlinds == 1 && (stepCount == 512)) {
    OpeningPosition = stepCount + 512;
  }
  if (OpenBlinds == 1 && (stepCount == 1024)) {
    OpeningPosition = stepCount + 512;
  }
  if (OpenBlinds == 1 && (stepCount == 1536)) {
    OpeningPosition = stepCount + 512;
  }
  if (OpenBlinds == 1 && (stepCount == 2048)) {
    OpenBlinds = 0;
  }

  //Close Blinds Position

  //if (CloseBlinds == 1 && (stepCount >= 2048)) {
  //ClosingPosition = stepCount - 512;
  //}
  //if (CloseBlinds == 1 && (stepCount == 1536)) {
  //  ClosingPosition = stepCount - 512;
  //}
  //if (CloseBlinds == 1 && (stepCount == 1024)) {
  //  ClosingPosition = stepCount - 512;
  //}
  //if (CloseBlinds == 1 && (stepCount >= 2048)) {
  //  ClosingPosition = stepCount - 512;
  //}
  //if (CloseBlinds == 1 && (stepCount == 512)) {
  //  ClosingPosition = stepCount - 512;
  //}

  if (CloseBlinds == 1 && (stepCount <= 0)) {
    CloseBlinds = 0;
  }

  if (OpenBlinds == 1) {

    digitalWrite(Phase1, LOW);
    digitalWrite(Phase2, LOW);
    digitalWrite(Phase3, HIGH);
    digitalWrite(Phase4, HIGH);
    delay (2);

    stepCount++;

    digitalWrite(Phase1, HIGH);
    digitalWrite(Phase2, LOW);
    digitalWrite(Phase3, LOW);
    digitalWrite(Phase4, HIGH);
    delay (2);

    stepCount++;

    digitalWrite(Phase1, HIGH);
    digitalWrite(Phase2, HIGH);
    digitalWrite(Phase3, LOW);
    digitalWrite(Phase4, LOW);
    delay (2);

    stepCount++;

    digitalWrite(Phase1, LOW);
    digitalWrite(Phase2, HIGH);
    digitalWrite(Phase3, HIGH);
    digitalWrite(Phase4, LOW);
    delay (2);

    stepCount++;

    if (stepCount == OpeningPosition) {
      OpenBlinds = 0;
      CloseBlinds = 0;
      digitalWrite(Phase1, LOW);
      digitalWrite(Phase2, LOW);
      digitalWrite(Phase3, LOW);
      digitalWrite(Phase4, LOW);
      delay(500);

    }

  };

  if (digitalRead(RunClose, HIGH)) {
    ClosingPosition = 2048;
    CloseBlinds = 1;
    OpenBlinds = 0;
  }

  if (CloseBlinds == 1) {
    digitalWrite(Phase1, LOW);
    digitalWrite(Phase2, HIGH);
    digitalWrite(Phase3, HIGH);
    digitalWrite(Phase4, LOW);
    delay (2);

    stepCount--;

    digitalWrite(Phase1, HIGH);
    digitalWrite(Phase2, HIGH);
    digitalWrite(Phase3, LOW);
    digitalWrite(Phase4, LOW);
    delay (2);

    stepCount--;

    digitalWrite(Phase1, HIGH);
    digitalWrite(Phase2, LOW);
    digitalWrite(Phase3, LOW);
    digitalWrite(Phase4, HIGH);
    delay (2);

    stepCount--;

    digitalWrite(Phase1, LOW);
    digitalWrite(Phase2, LOW);
    digitalWrite(Phase3, HIGH);
    digitalWrite(Phase4, HIGH);
    delay (2);

    stepCount--;

    if (stepCount == ClosingPosition) {
      CloseBlinds = 0;
      OpenBlinds = 0;
      digitalWrite(Phase1, LOW);
      digitalWrite(Phase2, LOW);
      digitalWrite(Phase3, LOW);
      digitalWrite(Phase4, LOW);
    }
  };
}


