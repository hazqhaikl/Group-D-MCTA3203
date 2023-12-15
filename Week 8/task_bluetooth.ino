#include <DHT.h>;
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN 7
char receivedValue = 0;
float temp;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  temp=dht.readTemperature();
  Serial.println(temp);

  if (Serial.available() > 0) {
    receivedValue = Serial.read();

    if (receivedValue == '1')
      digitalWrite(LED_PIN,HIGH);
    else if(receivedValue == '0')
      digitalWrite(LED_PIN,LOW);
  }
  delay(1000);
}
