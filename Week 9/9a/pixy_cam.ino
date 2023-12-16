#include <Pixy.h>
Pixy pixy;

#include <Servo.h>
Servo myservo;

void setup() {
  Serial.begin(9600);
  pixy.init();
  myservo.attach(8);
}
void loop() {
  int blocks = pixy.getBlocks();
  if (blocks) {
    for (int i = 0; i < blocks; i++) {
      Serial.print("Block ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print("Signature: ");
      Serial.print(pixy.blocks[i].signature);
      Serial.print(" X: ");
      Serial.print(pixy.blocks[i].x);
      Serial.print(" Y: ");
      Serial.println(pixy.blocks[i].y);

      if (pixy.blocks[i].signature == 1) {
        myservo.write(0);
      } else if (pixy.blocks[i].signature == 2) {
          myservo.write(90);
      } else if (pixy.blocks[i].signature == 3) {
          myservo.write(180);
      }
      delay(100);
    }
  }
}
