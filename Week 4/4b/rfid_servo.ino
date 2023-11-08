#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5
#define LED_R 4
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo;
 
void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(3);
  myServo.write(0);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}
void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "43 A6 9F 95")
  {
    Serial.println("A");
    delay(500);
    digitalWrite(LED_G, HIGH);
    myServo.write(90);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  }
  else   {
    Serial.println("D");
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
  }
}
