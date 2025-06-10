#define BLYNK_TEMPLATE_ID "TMPL3JaaGQxgc"
#define BLYNK_TEMPLATE_NAME "Parking System"
#define BLYNK_AUTH_TOKEN "6KruAsSVWOYmyXDTI1jibYTVtfCuEa3D"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Replace with your network credentials
char ssid[] = "devilliers";
char pass[] = "88888888";

#define RXD2 16 // ESP32 pin receiving data from Arduino TX
#define TXD2 17 // Optional, not used here

String inputString = "";
int slots[3] = {0, 0, 0};
int available = 0;

void setup() {
  Serial.begin(115200);     // Debug output
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Arduino -> ESP32 Serial

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  readFromArduino();
}

void readFromArduino() {
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    if (inChar == '\n') {
      parseData(inputString);
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
}

void parseData(String data) {
  // Expected: "2,1,0,1"
  int values[4];
  int idx = 0;

  while (data.length() > 0 && idx < 4) {
    int comma = data.indexOf(',');
    if (comma == -1) {
      values[idx++] = data.toInt();
      break;
    } else {
      values[idx++] = data.substring(0, comma).toInt();
      data = data.substring(comma + 1);
    }
  }

  available = values[0];
  for (int i = 0; i < 3; i++) slots[i] = values[i + 1];

  // Send to Blynk
  Blynk.virtualWrite(V0, available);
  Blynk.virtualWrite(V1, slots[0] ? "Occupied" : "Empty");
  Blynk.virtualWrite(V2, slots[1] ? "Occupied" : "Empty");
  Blynk.virtualWrite(V3, slots[2] ? "Occupied" : "Empty");

  Serial.println("Updated Blynk");
}