<p align="center">
  <img src="images/university-logo.jpeg" alt="Namal University" width="150">
</p>

<h1 align="center">IoT Lab 6 - SPI-Based Peripheral Communication and RFID Identification</h1>

<p align="center">
  <strong>Namal University, Mianwali</strong><br>
  Department of Electrical Engineering<br>
  EE455L - Internet of Things (Lab)
</p>

| | |
|---|---|
| **Student** | Ali Haider (NUM-BSEE-2022-13) |
| **Instructor** | Engr. Junaid Ashraf |
| **Date** | 13th April, 2026 |

---

## Learning Objectives

- Configure SPI protocol on ESP32
- Interface RFID (MFRC522) module with ESP32
- Read RFID tag UID and process identification
- Implement a basic secure identification mechanism
- Display output on Serial Monitor / OLED

---

## Hardware Requirements

| Component | Details |
|-----------|---------|
| Microcontroller | ESP32-S3 |
| RFID Module | MFRC522 |
| OLED Display | SSD1306 (128x64, I2C) |
| Buzzer | Active buzzer module |
| LED | Built-in or external |
| RFID Tags | Mifare 13.56MHz cards/fobs |

---

## Pin Connections

### RFID (SPI)

| MFRC522 Pin | ESP32-S3 Pin |
|-------------|--------------|
| SDA (SS)    | GPIO 5       |
| SCK         | GPIO 36      |
| MOSI        | GPIO 35      |
| MISO        | GPIO 37      |
| RST         | GPIO 4       |

### OLED (I2C)

| OLED Pin | ESP32-S3 Pin |
|----------|--------------|
| SDA      | GPIO 8       |
| SCL      | GPIO 9       |

### Other

| Component | ESP32-S3 Pin |
|-----------|--------------|
| Buzzer    | GPIO 10      |
| LED       | GPIO 2       |

### RFID Module Internal Circuit

<p align="center">
  <img src="images/rfid-internal-circuit.jpeg" alt="RFID Internal Circuit" width="500">
</p>

---

## Tasks

### Task 1 - SPI Configuration and RFID Interfacing

Basic RFID UID reader that detects tags and displays their UID in hexadecimal format on the Serial Monitor.

**Folder:** `Task1_RFID_Reader/`

**Libraries Required:**
- `SPI.h` (built-in)
- `MFRC522.h` (install via Library Manager)

#### Output

<p align="center">
  <img src="images/task1-output.jpeg" alt="Task 1 Output" width="500">
</p>

```
RFID UID Reader
Scan your card...
UID: A1 B2 C3 D4
```

---

### Task 2 - Secure Identification and System Verification

Advanced RFID authentication system with:
- Authorized student database (UID -> Roll No + Name)
- OLED display for real-time output
- Buzzer feedback (short beep for granted, long beep for denied)
- LED indicator for access status

**Folder:** `Task2_Secure_Identification/`

**Libraries Required:**
- `SPI.h` (built-in)
- `MFRC522.h`
- `Wire.h` (built-in)
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`

#### Output

<p align="center">
  <img src="images/task2-output.jpeg" alt="Task 2 Output" width="500">
</p>

**Authorized:**
```
UID: 67 9E 37 25
Roll: NUM-BSEE-2022-13
Name: Malik Uzair
Access Granted
```

**Unauthorized:**
```
UID: AA BB CC DD
Access Denied
```

---

## How to Upload

1. Open Arduino IDE
2. Install required libraries via **Sketch > Include Library > Manage Libraries**
3. Select board: **ESP32-S3 Dev Module**
4. Select the correct COM port
5. Upload the `.ino` file from the respective task folder

---

## Theory

### SPI Communication
SPI (Serial Peripheral Interface) is a synchronous, full-duplex protocol using four lines:
- **MOSI** - Master Out Slave In
- **MISO** - Master In Slave Out
- **SCK** - Clock Signal
- **SS** - Slave Select

### RFID Identification
The MFRC522 operates at 13.56MHz. When a tag enters the reader's field, it responds with its unique UID. The ESP32 reads this UID via SPI and compares it against an authorized database to grant or deny access.
