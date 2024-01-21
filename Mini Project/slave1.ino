// Arduino Mega (Slave 1)

// RFID
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);

// OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIDTH 64
#define HEIGHT 128
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
static const unsigned char image_loading_10px_bits[] PROGMEM = {0x7f,0x00,0x41,0x00,0x5d,0x00,0x2a,0x00,0x14,0x00,0x14,0x00,0x22,0x00,0x49,0x00,0x5d,0x00,0x7f,0x00};

//SERVO
#include <Servo.h>
Servo door;

#define SPEAKER 12
#define RELAY1 6
#define RELAY2 7

bool accessRFID = false;
int angle = 0;
float mass = 0.0;
int flowDelay = 0;
int bluetoothData;
bool cond = 1;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(SPEAKER, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  door.attach(5);
  door.write(angle);  // Closing the door

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  starting_oled(); // Greeting
}

void loop() {
  accessRFID=access(accessRFID);
  if(accessRFID && cond){
    delay(1000);  // Safety (Wait for a while before closing the door)
    door.write(angle);
    if (Serial2.available() >= sizeof(float)) { // Receive mass data from master
      mass=readMass();
      if (mass > 10) {
        flowDelay=fillinDuration(mass); // Convert mass into fill in water delay
        fillinWater();
      }
    }
  }
  // Read serial data received from master
  if (Serial2.available() > 0) {
    char state2 = Serial2.read();
    if (state2 == 'D') {  // Remove water from the drum
      filloutWater();
    }else if (state2 == 'G') {  // Opening the tetris game
      cond=0;
      delay(500);
      startGame();
      filloutWater();
    }else if (state2 == 'H') {  // Opening the door after finish spinning
      delay(1000);
      angle = 90;
      door.write(angle);
    }
  }
}

void starting_oled() {
  display.setRotation(3);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("AUTOMATIC");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(11, 37);
  display.setTextWrap(false);
  display.print("WASHING");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(11, 47);
  display.setTextWrap(false);
  display.print("MACHINE");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(11, 74);
  display.setTextWrap(false);
  display.println("GROUP D");
  display.drawBitmap(26, 96, image_loading_10px_bits, 10, 10, 1);
  display.drawRect(0, 0, 64, 128, 1);
  display.drawLine(5, 32, 57, 32, 1);
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
}
bool access(bool x) {
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return x;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return x;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "43 A6 9F 95") {
    Serial.println("REGISTERED CARD");  // Access Granted
    angle = 90;
    Serial2.write('A');
    accessGranted();
    return true;
  }
  else {
    Serial.println("NOT REGISTERED CARD");  // Access Denied
    angle = 0;
    Serial2.write('B');
    accessDenied();
    return false;
  }
}

float readMass() {
  Serial2.readBytes((char*)&mass, sizeof(mass));
  Serial.print("Mass: ");
  Serial.print(mass);
  Serial.println(" g");
  return mass;
}
int fillinDuration(float mass) {
  flowDelay = map(mass, 0, 1000, 0, 30000);
  Serial.print("Flow delay: ");
  Serial.print(flowDelay);
  Serial.println(" sec");
  return flowDelay;
}
void accessGranted() {
  display.clearDisplay();
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(14, 55);
  display.setTextWrap(false);
  display.print("ACCESS");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(11, 71);
  display.setTextWrap(false);
  display.print("GRANTED");
  display.drawRect(0, 0, 64, 128, 1);
  display.display();
  
  tone(SPEAKER, 1000);
  delay(100);
  noTone(SPEAKER);
  delay(100);
  tone(SPEAKER, 1000);
  delay(100);
  noTone(SPEAKER);
  delay(1700);

  display.clearDisplay();
  display.display();
}
void accessDenied() {
  display.clearDisplay();  
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(14, 55);
  display.setTextWrap(false);
  display.print("ACCESS");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(14, 71);
  display.setTextWrap(false);
  display.print("DENIED");
  display.drawRect(0, 0, 64, 128, 1);
  display.display();

  tone(SPEAKER, 1000);
  delay(1000);
  noTone(SPEAKER);
  delay(1000);

  display.clearDisplay();
  display.display();
}
void fillinWater() {
  display.clearDisplay(); 
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(12, 50);
  display.setTextWrap(false);
  display.print("FILL IN");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(18, 64);
  display.setTextWrap(false);
  display.print("WATER");
  display.drawRect(0, 0, 64, 128, 1);
  display.display();

  delay(500);
  tone(SPEAKER, 1000);
  delay(700);
  noTone(SPEAKER);
  delay(300);
  tone(SPEAKER, 1000);
  delay(700);
  noTone(SPEAKER);
  delay(500);
  angle = 0;
  door.write(angle);
  delay(500);
  digitalWrite(RELAY1, LOW);
  Serial3.write('L');
  delay(flowDelay);
  digitalWrite(RELAY1, HIGH);
  Serial2.write('S');

  display.clearDisplay();
  display.display();
}
void filloutWater() {
  display.clearDisplay(); 
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(9, 50);
  display.setTextWrap(false);
  display.print("FILL OUT");
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(18, 64);
  display.setTextWrap(false);
  display.print("WATER");
  display.drawRect(0, 0, 64, 128, 1);
  display.display();

  delay(500);
  digitalWrite(RELAY2, LOW);
  delay(5000);
  digitalWrite(RELAY2, HIGH);
  Serial2.write('W');

  display.clearDisplay();
  display.display();
}
