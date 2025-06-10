#include <LiquidCrystal.h>

// Initialize LCD with RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Flex sensor analog pins
int flexPins[5] = {A0, A1, A2, A3, A4};

// LED pin
const int ledPin = 8;

// Threshold for detecting bend
const int threshold = 200;  // Adjust this value based on your sensor readings

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // 16 columns, 2 rows
  pinMode(ledPin, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(flexPins[i], INPUT);
  }

  lcd.setCursor(0, 0);
  lcd.print("Helping Glove");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValues[5];
  bool anyBent = false;
  bool allBent = true;

  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(flexPins[i]);
    Serial.print("Flex ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValues[i]);

    if (sensorValues[i] < threshold) {
      allBent = false; // If any sensor is below threshold, not all are bent
    } else {
      anyBent = true;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);

  if (allBent || !anyBent) {
    lcd.print("Status: OK");
  } else {
    if (sensorValues[0] >= threshold) {
      lcd.print("Need Water");
    } else if (sensorValues[1] >= threshold) {
      lcd.print("Need Medicine");
    } else if (sensorValues[2] >= threshold) {
      lcd.print("Need Help");
    } else if (sensorValues[3] >= threshold) {
      lcd.print("Need Food");
    } else if (sensorValues[4] >= threshold) {
      lcd.print("Need Painkiller");
    }
  }

  // LED ON if any finger bent (request), OFF otherwise
  digitalWrite(ledPin, anyBent && !allBent ? HIGH : LOW);

  delay(500); // Small delay for readability
}