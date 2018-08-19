#include <Servo.h>
#include "SPI.h"
#include "MFRC522.h"


#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8
#define LED A0
Servo servo;


MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(LED, OUTPUT);

  servo.attach(8);
  servo.write(0);
  delay(2000);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  
  Serial.print("Card key: ");

  Serial.println(strID);
    Serial.println("Name: Nahid Hasan Limon");
    Serial.println("ID:13143103061");
    Serial.println("Inatke:27th");
    Serial.println("Section:02");

  //test LED
  if (strID.indexOf("06:9C:3F:54") >= 0) {
  digitalWrite(LED,HIGH);
  servo.write(90);
  delay(2000);
  servo.write(0);
  delay(1000);
} else{
  digitalWrite(LED,LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
