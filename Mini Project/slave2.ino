// Arduino Uno (Slave 2)

// Water Level Sensor
#define POWER_PIN 7
#define SIGNAL_PIN A3
int value = 0;

// LCD Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  lcd.setCursor(1, 0);
  lcd.print("Hello");
}

void loop() {
  // Serial Data Received from Slave 1
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == 'L') {  // Turning on Water Level Sensor
      for (int i = 0; i < 50; i++) {
        digitalWrite(POWER_PIN, HIGH);
        delay(10);
        value = analogRead(SIGNAL_PIN);
        digitalWrite(POWER_PIN, LOW);

        lcd.setCursor(1, 0);
        lcd.print("Sensor value: ");
        lcd.print(value);

        Serial.print("Sensor value: ");
        Serial.println(value);
        if (value >= 130) {
          Serial.println("Warning!");  // Safety
        }
        delay(1000);
      }
    }
  }
}
