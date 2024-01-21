// Arduino Uno (Master)

// Load Cell
#include "HX711.h"
HX711 scale;
uint8_t dataPin = 11;
uint8_t clockPin = 12;
float mass = 0;

// Motor Driver Shield
#include "Cytron_Shield3AMotor.h"
Shield3AMotor motor(SIGNED_MAGNITUDE);

// GSM Module
#include <SoftwareSerial.h>
SoftwareSerial SIM900A(8, 9);

// Pushbutton
const int buttonPin = 13;
bool buttonState = 0;

// LED
#define GREEN 2
#define RED 3

void setup() {
  SIM900A.begin(9600);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, HIGH);

  scale.begin(dataPin, clockPin);
  scale.set_scale(220.0983);  // load cell factor 10 KG
  scale.tare(20);
}

void loop() {
  if (scale.is_ready()) {
    buttonState = digitalRead(buttonPin);
    mass = scale.get_units(1);

    if (buttonState == HIGH && mass > 10) {
      Serial.write((byte*)&mass, sizeof(mass));  // Send mass to slave 1
      delay(500);
    }
  }
  // Read serial data received from slave 1
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == 'A') {  // RFID Access Granted
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    } else if (receivedChar == 'B') {  // RFID Access Denied
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    } else if (receivedChar == 'S') {  // Start Washing (Agitator Rotating)
      delay(500);
      startWashing();
    } else if (receivedChar == 'W') { // Start Spinning (Drum Rotating)
      startSpinning();
    }
  }
}

void startWashing() {
  Serial.write('G');
  for (int i = 0; i < 3; i++) {
    motor.control(100, 0);
    delay(5000);
    motor.control(-100, 0);
    delay(5000);
    motor.control(0, 0);
  }
  Serial.write('D');
}

void startSpinning() {
  for (int i = 0; i < 3; i++) {
    motor.control(0, 100);
    delay(5000);
    motor.control(0, -100);
    delay(5000);
    motor.control(0, 0);
  }
  SendMessage();
  Serial.write('H');
}

void SendMessage() {
  Serial.println("Sending Message");
  SIM900A.println("AT");  // Check if GSM module responds
  delay(1000);
  SIM900A.println("AT+CMGF=1");  // Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+60175934009\"\r");  // Mobile phone number to send message
  delay(1000);
  Serial.println("Set SMS Content");
  SIM900A.println("HAI BAJU ANDA DAH SIAP! BILA NAK AMBIK ?");  // Message content
  delay(100);
  Serial.println("Finish");
  SIM900A.println((char)26);
  delay(1000);
  Serial.println("Message has been sent");
}
