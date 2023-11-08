#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

int16_t ax, ay, az, gx, gy, gz;
int scalingFactor = 20;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int x = map(ax, -17000, 17000, 0, scalingFactor); 
  int y = map(ay, -17000, 17000, 0, scalingFactor);
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  delay(300);
}
