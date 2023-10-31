const int potentio = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potentio);
  Serial.println(potValue);
  delay(1000);
}
