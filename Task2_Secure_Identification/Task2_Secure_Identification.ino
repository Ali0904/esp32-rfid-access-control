#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= PIN CONFIG =================
// RFID (SPI)
#define SS_PIN   5
#define RST_PIN  4
#define SCK_PIN  36
#define MISO_PIN 37
#define MOSI_PIN 35

// OLED (I2C)
#define SDA_PIN 8
#define SCL_PIN 9

// Built-in LED (ESP32-S3 may vary, try 2 or 48 if needed)
#define LED_BUILTIN 2

// Buzzer
#define BUZZER_PIN 10

// ================= OLED CONFIG =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ================= STUDENT DATABASE =================
struct Student {
    String uid;
    String roll;
    String name;
};

Student students[] = {
    {"67 9E 37 25", "NUM-BSEE-2022-13", "Malik Uzair"},
    {"47 8E C0 2E", "NUM-BSEE-2022-25", "Badar Hafeez"}
};

// ================= GET STUDENT =================
Student getStudent(String uid) {
    for (int i = 0; i < sizeof(students) / sizeof(students[0]); i++) {
        if (uid.equalsIgnoreCase(students[i].uid)) {
            return students[i];
        }
    }
    return {"", "UNKNOWN", "UNKNOWN"};
}

// ================= BUZZER =================
void beep(int duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

// ================= OLED DISPLAY =================
void showStudent(Student s, String uid) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("RFID DETECTED");
    display.setCursor(0, 12);
    display.println("UID:");
    display.println(uid);
    display.setCursor(0, 28);
    display.println("Roll:");
    display.println(s.roll);
    display.setCursor(0, 44);
    display.println("Name:");
    display.println(s.name);
    display.display();
}

// ================= SETUP =================
void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // OLED INIT
    Wire.begin(SDA_PIN, SCL_PIN);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("RFID SYSTEM STARTING");
    display.display();
    delay(1500);

    // SPI INIT
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // RFID INIT
    mfrc522.PCD_Init();
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("READY");
    display.setCursor(0, 30);
    display.println("SCAN CARD...");
    display.display();
    Serial.println("Scan RFID Tag...");
}

// ================= LOOP =================
void loop() {
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    String uidStr = "";

    // UID FORMATTING
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
            uidStr += "0";
        }
        String hexByte = String(mfrc522.uid.uidByte[i], HEX);
        hexByte.toUpperCase();
        uidStr += hexByte;
        if (i < mfrc522.uid.size - 1) uidStr += " ";
    }

    Serial.println("UID: " + uidStr);

    Student s = getStudent(uidStr);

    if (s.roll != "UNKNOWN") {
        Serial.println("Roll: " + s.roll);
        Serial.println("Name: " + s.name);
        Serial.println("Access Granted");
        digitalWrite(LED_BUILTIN, HIGH);
        beep(200);
        showStudent(s, uidStr);
    } else {
        Serial.println("Access Denied");
        digitalWrite(LED_BUILTIN, LOW);
        beep(600);
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("ACCESS DENIED");
        display.setCursor(0, 30);
        display.println(uidStr);
        display.display();
    }

    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("SCAN CARD...");
    display.display();
    mfrc522.PICC_HaltA();
}
