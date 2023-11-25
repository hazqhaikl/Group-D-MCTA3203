void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL, Time,Started Time, Register value");
  Serial.println("RESETTIMER");
}

void loop() {
  int sensorValue = analogRead(A1);
  Serial.print("DATA, TIME, TIMER,");
  Serial.println(sensorValue);
  delay(1500);
}
