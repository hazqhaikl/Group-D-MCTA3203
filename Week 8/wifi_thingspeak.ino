#include <DHT.h>

#include <WiFi.h>
#include <HTTPClient.h>

String apiKey = "2QI4HLU34KLANK2C";

const char *ssid =  "Wafi";
const char *pass =  "12345678";
const char* server = "api.thingspeak.com/update";

#define DHTPIN 32

DHT dht(DHTPIN, DHT11);

WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (WiFi.status()== WL_CONNECTED) {
  delay(2000);
  http.begin(client, server);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);          
  int httpResponseCode = http.POST(httpRequestData);
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.print(h);
  Serial.println("%. Send to Thingspeak.");
  http.end();
  delay(2000);
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
