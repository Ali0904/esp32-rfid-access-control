#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// SPI Pins for ESP32-S3
#define SS_PIN   5
#define RST_PIN  4
#define SCK_PIN  36
#define MISO_PIN 37
#define MOSI_PIN 35

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("RFID UID Reader");

    // Initialize SPI
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // Initialize RFID
    mfrc522.PCD_Init();
    Serial.println("Scan your card...");
}

void loop() {
    // Check for new card
    if (!mfrc522.PICC_IsNewCardPresent()) return;

    // Read card
    if (!mfrc522.PICC_ReadCardSerial()) return;

    Serial.print("UID: ");

    // Print UID in HEX
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
            Serial.print("0");
        }
        String hexByte = String(mfrc522.uid.uidByte[i], HEX);
        hexByte.toUpperCase();
        Serial.print(hexByte);
        Serial.print(" ");
    }

    Serial.println();

    // Stop reading same card repeatedly
    mfrc522.PICC_HaltA();
    delay(1000);
}
