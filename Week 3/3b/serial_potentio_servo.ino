#include <Servo.h>

Servo myservo;
int potPin = A0;
int potValue = 0;
int servoAngle = 0;
char command;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    if (command == 'H') {
      stopServo();
    }
  }

  potValue = analogRead(potPin);
  servoAngle = map(potValue, 0, 1023, 0, 180);
  myservo.write(servoAngle);

  delay(50);
}

void stopServo() {
  myservo.detach();
  while (true) {
    if (Serial.available() > 0) {
      command = Serial.read();
      if (command == 'R') {
        myservo.attach(9);
        Serial.println(servoAngle);
        break;
      }
    }
  }
}
