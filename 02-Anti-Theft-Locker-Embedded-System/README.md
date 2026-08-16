# 🔒 Anti-Theft Locker Embedded System

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-Embedded%20C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![Simulation](https://img.shields.io/badge/Simulation-Wokwi-8A2BE2)
![Architecture](https://img.shields.io/badge/Architecture-Finite%20State%20Machine-6F42C1)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)
![License](https://img.shields.io/badge/License-MIT-green)

<p align="center">

[▶️ **Run Live Wokwi Simulation**](https://wokwi.com/projects/472327786263779329)

</p>

> An Arduino UNO-based electronic access-control system with PIN authentication, 4×4 keypad input, masked password display, SG90 servo locking, automatic relocking, failed-attempt detection, and a security lockout alarm.

---

## 📑 Table of Contents

1. [Overview](#-overview)
2. [Project Objectives](#-project-objectives)
3. [Key Features](#-key-features)
4. [System Architecture](#️-system-architecture)
5. [Finite State Machine & Authentication Logic](#-finite-state-machine--authentication-logic)
6. [Servo Locking Mechanism](#-servo-locking-mechanism)
7. [Automatic Relocking](#️-automatic-relocking)
8. [Failed-Attempt Detection & Security Lockout](#-failed-attempt-detection--security-lockout)
9. [LED & Buzzer Status](#-led--buzzer-status)
10. [LCD User Interface](#-lcd-user-interface)
11. [Keypad Operation](#-keypad-operation)
12. [Pin Configuration](#-pin-configuration)
13. [Firmware Parameters](#️-firmware-parameters)
14. [Firmware Architecture](#-firmware-architecture)
15. [Technology Stack](#️-technology-stack)
16. [How to Run](#️-how-to-run)
17. [Testing & Validation](#-testing--validation)
18. [Screenshots](#-screenshots)
19. [Repository Structure](#-repository-structure)
20. [Security Considerations](#-security-considerations)
21. [Project Limitations](#️-project-limitations)
22. [Future Enhancements](#-future-enhancements)
23. [Skills & Learning Outcomes](#-skills--learning-outcomes)
24. [Author](#-author)
25. [License](#-license)

---

## 📌 Overview

The **Anti-Theft Locker Embedded System** is an Arduino UNO-based electronic access-control prototype demonstrating how authentication, hardware interfacing, actuator control, timing logic, and security mechanisms integrate into a complete embedded system.

The system uses a **4×4 matrix keypad** for PIN entry, a **16×2 I2C LCD** for user feedback, an **SG90 servo motor** to simulate the physical locking mechanism, green and red LEDs for status indication, and a piezo buzzer for audible feedback and security alarms.

The firmware is structured around a **Finite State Machine (FSM)** with three states — `LOCKED_STATE`, `UNLOCKED_STATE`, and `LOCKOUT_STATE`. A correct PIN temporarily unlocks the system; after approximately **5 seconds** it automatically relocks. Three consecutive incorrect attempts trigger a **10-second security lockout**, during which keypad input is blocked and an alternating alarm activates.

The complete prototype has been implemented and functionally tested in the **Wokwi electronics simulator**.

---

## 🎯 Project Objectives

- Implement PIN-based electronic locker authentication
- Interface a 4×4 matrix keypad with Arduino UNO
- Mask password input on a 16×2 I2C LCD
- Control an SG90 servo as a simulated locking mechanism
- Implement a finite state machine for system control
- Automatically relock the locker after successful authentication
- Detect consecutive failed authentication attempts and trigger a security lockout
- Provide visual (LED) and audible (buzzer) status feedback
- Display system information through the Serial Monitor
- Validate the complete embedded system through simulation

---

## ✨ Key Features

**Authentication & Access**
- PIN-based authentication with a 4×4 matrix keypad
- Password masking (`*`) on the 16×2 I2C LCD, with `*` to clear and `#` to submit
- Three-strike failed-attempt detection with a 10-second security lockout

**Locking Mechanism**
- SG90 servo-based locking — unlock position 90°, lock position 0°
- Automatic relocking approximately 5 seconds after a successful unlock

**Feedback & Alerts**
- Green LED on successful unlock; red LED during lock/alarm states, flashing during lockout
- Distinct buzzer tones for keypress (2000 Hz), access granted, access denied (400 Hz), and the alternating 1200/800 Hz lockout alarm

**System & Tooling**
- Finite State Machine architecture (`LOCKED` / `UNLOCKED` / `LOCKOUT`)
- 9600-baud Serial Monitor telemetry
- Fully validated in Wokwi simulation

---

## 🏗️ System Architecture

```text
                           ┌─────────────────────┐
                           │     Arduino UNO     │
                           │     ATmega328P      │
                           └──────────┬──────────┘
                                      │
              ┌───────────────────────┼────────────────────────┐
              │                       │                        │
              ▼                       ▼                        ▼
     ┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
     │   4×4 Matrix    │     │   16×2 I2C LCD  │     │ UART Serial     │
     │     Keypad      │     │     Display     │     │    Monitor      │
     └────────┬────────┘     └─────────────────┘     └─────────────────┘
              │
              ▼
     ┌─────────────────────┐
     │ Password Processing │
     │ & Authentication    │
     └──────────┬──────────┘
                │
                ▼
     ┌──────────────────────┐
     │ Finite State Machine │
     │  LOCKED / UNLOCKED /  │
     │       LOCKOUT         │
     └──────────┬──────────┘
                │
       ┌────────┼──────────────┬─────────────┐
       │        │              │             │
       ▼        ▼              ▼             ▼
    ┌──────┐ ┌──────┐     ┌────────┐    ┌────────┐
    │ SG90 │ │Buzzer│     │ Green  │    │  Red   │
    │Servo │ │      │     │  LED   │    │  LED   │
    └──┬───┘ └──────┘     └────────┘    └────────┘
       │
       ▼
 ┌────────────────────┐
 │ Locker / Deadbolt  │
 │ Status Simulation  │
 └────────────────────┘
```

---

## 🔄 Finite State Machine & Authentication Logic

```text
                         ┌──────────────────────┐
                         │     LOCKED_STATE      │
                         │  Servo = 0°, Red LED  │
                         └──────────┬───────────┘
                                    │
                          Enter PIN → Press '#'
                                    │
                         ┌──────────▼───────────┐
                         │   PIN VALIDATION      │
                         └───────┬────────┬─────┘
                              VALID    INVALID
                                 │        │
                                 ▼        ▼
                    ┌────────────────┐  ┌──────────────────┐
                    │ UNLOCKED_STATE │  │  ACCESS DENIED    │
                    │ Servo = 90°    │  │  Fail Count + 1   │
                    │ Green LED ON   │  └────────┬──────────┘
                    └───────┬────────┘           │
                            │              Attempts < 3 → Try again
                        ~5 sec                    │
                            │              Attempts ≥ 3
                            ▼                      │
                     LOCKED_STATE                  ▼
                                      ┌────────────────────┐
                                      │   LOCKOUT_STATE     │
                                      │ Servo = 0°           │
                                      │ Keypad blocked       │
                                      │ Alarm + flashing LED │
                                      └─────────┬───────────┘
                                                 │
                                              10 sec
                                                 │
                                                 ▼
                                     Reset counter → LOCKED_STATE
```

**Default prototype PIN:** `1234`

> **Note:** The PIN is intentionally visible in the source because this is an educational prototype — see [Security Considerations](#-security-considerations) for why this would need to change in a production system.

On a valid PIN, the system moves the servo to 90°, turns the green LED on, plays a confirmation tone, and starts the auto-lock timer. On an invalid PIN, it increments the failed-attempt counter, plays a warning tone, and shows the remaining attempts — until the third failure triggers `LOCKOUT_STATE`.

---

## 🔓 Servo Locking Mechanism

The **SG90 servo motor** simulates the physical locker deadbolt.

| System State | Servo Position | Meaning |
|---|---:|---|
| `LOCKED_STATE` | 0° | Locker locked |
| `UNLOCKED_STATE` | 90° | Locker unlocked |
| `LOCKOUT_STATE` | 0° | Locker remains locked |

```cpp
const byte LOCK_ANGLE = 0;
const byte UNLOCK_ANGLE = 90;
```

---

## ⏱️ Automatic Relocking

After successful authentication, the firmware enters `UNLOCKED_STATE` and records the current time using `millis()`:

```cpp
const unsigned long AUTO_LOCK_DELAY = 5000UL;
```

The sequence is: access granted → servo to 90°, green LED on → `millis()`-based timer starts → after ~5 seconds → servo to 0°, red LED on → back to `LOCKED_STATE`.

The primary relock and lockout timers use `millis()` rather than blocking `delay()` calls.

> **Implementation note:** short `delay()` calls are still used in some UI and buzzer feedback routines — only the relock and lockout timing are fully non-blocking.

---

## 🚨 Failed-Attempt Detection & Security Lockout

```cpp
const byte MAX_FAILED_ATTEMPTS = 3;
const unsigned long LOCKOUT_TIME = 10000UL;
```

After the third failed authentication attempt, the system enters `LOCKOUT_STATE`:

- Servo remains at 0°; keypad input is blocked
- Red LED alternates every 250 ms; green LED stays off
- Buzzer alternates between 1200 Hz and 800 Hz
- LCD displays `TOO MANY` / `ATTEMPTS!`
- After 10 seconds, the failed-attempt counter resets and the system returns to `LOCKED_STATE`

The alarm and LED alternation are driven by a `millis()`-based 250 ms timer.

---

## 🚦 LED & Buzzer Status

| System Condition | Green LED | Red LED | Buzzer |
|---|---|---|---|
| `LOCKED_STATE` | OFF | ON | Silent |
| Keypress | State dependent | State dependent | 2000 Hz / 50 ms |
| `UNLOCKED_STATE` | ON | OFF | 1000 Hz → 1500 Hz |
| Access denied | OFF | ON | 400 Hz / 500 ms |
| `LOCKOUT_STATE` | OFF | Alternating | 1200 Hz ↔ 800 Hz |

---

## 📺 LCD User Interface

| Screen | Content |
|---|---|
| Standby | `ENTER PASSWORD:` / `PASS:****` |
| Access Granted | `ACCESS GRANTED` / `LOCKER OPEN` |
| Access Denied | `ACCESS DENIED` / `TRIES LEFT: 2` |
| Security Lockout | `TOO MANY` / `ATTEMPTS!` |
| Automatic Relock | `LOCKER LOCKED` / `SECURE` |
| Lockout Recovery | `LOCKOUT ENDED` / `TRY AGAIN` |

---

## 🔢 Keypad Operation

```text
┌─────┬─────┬─────┬─────┐
│  1  │  2  │  3  │  A  │
├─────┼─────┼─────┼─────┤
│  4  │  5  │  6  │  B  │
├─────┼─────┼─────┼─────┤
│  7  │  8  │  9  │  C  │
├─────┼─────┼─────┼─────┤
│  *  │  0  │  #  │  D  │
└─────┴─────┴─────┴─────┘
```

| Key | Function |
|---|---|
| `0–9` | Enter password digit |
| `*` | Clear current password input |
| `#` | Submit password |
| `A–D` | Ignored |

Password input is limited to a configured maximum length of **8 characters**.

---

## 🔌 Pin Configuration

| Component | Module Pin | Arduino UNO Pin | Function |
|---|---|---|---|
| 4×4 Matrix Keypad | Row 1–4 | D9, D8, D7, D6 | Matrix rows |
| 4×4 Matrix Keypad | Col 1–4 | D5, D4, D3, D2 | Matrix columns |
| I2C LCD | SDA / SCL | A4 / A5 | I2C data / clock |
| I2C LCD | VCC / GND | 5V / GND | Power |
| SG90 Servo | Signal | D10 | PWM control |
| SG90 Servo | VCC / GND | 5V / GND | Power |
| Piezo Buzzer | Positive / GND | D11 / GND | Audio output |
| Green LED | Anode | D12 via 220Ω | Unlock indicator |
| Red LED | Anode | D13 via 220Ω | Lock / alarm indicator |

---

## ⚙️ Firmware Parameters

| Parameter | Value | Description |
|---|---|---|
| Default PIN | `1234` | Prototype access PIN |
| Maximum Password Length | 8 characters | Maximum accepted input |
| Lock / Unlock Angle | 0° / 90° | Servo positions |
| Auto-Lock Delay | 5000 ms | Automatic relock interval |
| Maximum Failed Attempts | 3 | Attempts before lockout |
| Lockout Duration | 10000 ms | Security lockout duration |
| Alarm Frequencies | 1200 Hz / 800 Hz | Lockout alarm tones |
| Alarm Toggle Rate | 250 ms | Alarm/LED alternation |
| Keypress Tone | 2000 Hz / 50 ms | Key feedback |
| Access Denied Tone | 400 Hz / 500 ms | Warning tone |
| Access Granted Tone | 1000 Hz → 1500 Hz | Success feedback |
| UART Baud Rate | 9600 | Serial communication |
| LCD I2C Address / Size | `0x27` / 16×2 | Display config |

---

## 🧠 Firmware Architecture

```cpp
enum SystemState {
    LOCKED_STATE,
    UNLOCKED_STATE,
    LOCKOUT_STATE
};
```

| State | Responsibility |
|---|---|
| `LOCKED_STATE` | Normal secure standby and password authentication |
| `UNLOCKED_STATE` | Temporary authorized access |
| `LOCKOUT_STATE` | Security response after three failed attempts |

**Major firmware functions:**

```text
initializeHardware(), showWelcomeScreen(), showPasswordScreen()
processKey(), handleDigit(), handleSubmit(), handleClear()
checkPassword(), grantAccess(), lockLocker(), denyAccess()
triggerLockout(), handleLockout()
resetPasswordInput(), updatePasswordDisplay()
setLockedIndicators(), setUnlockedIndicators(), setAlarmIndicators(), stopAlarm()
beepKey(), beepAccessGranted(), beepAccessDenied()
printSystemStatus()
```

---

## 🛠️ Technology Stack

**Hardware:** Arduino UNO (ATmega328P) · 4×4 Matrix Keypad · SG90 Servo Motor · 16×2 I2C LCD (PCF8574) · Piezoelectric Buzzer · Green/Red LEDs · 220Ω Resistors

**Software & Libraries:** Embedded C/C++ · Arduino Core · `Keypad.h` · `Servo.h` · `Wire.h` · `LiquidCrystal_I2C.h` · Wokwi Electronics Simulator · Git/GitHub

---

## ▶️ How to Run

1. Open the live simulation: **[▶️ Run on Wokwi](https://wokwi.com/projects/472327786263779329)**
2. Click **Start Simulation**. The system initializes the keypad, LCD, servo, buzzer, LEDs, and Serial Monitor.
3. **Test successful authentication:** enter `1234#`. Expect `ACCESS GRANTED` → servo to 90°, green LED on → ~5-second timer → servo to 0°, red LED on → back to `LOCKED_STATE`.
4. **Test failed authentication:** enter an incorrect PIN (e.g. `9999#`) twice. Expect `ACCESS DENIED` with decreasing "tries left" on the LCD.
5. **Test security lockout:** enter a third incorrect PIN. Expect `TOO MANY ATTEMPTS!` on the LCD, keypad blocked, red LED alternating, buzzer alarm for 10 seconds, then automatic reset to `LOCKED_STATE`.

---

## 🧪 Testing & Validation

| Test Case | Input / Condition | Expected Result |
|---|---|---|
| Initial Boot | Start simulation | `LOCKED_STATE`, red LED ON, servo at 0° |
| Password Entry | Enter numeric PIN | LCD displays masked `*` characters |
| Clear Input | Press `*` | Password input cleared |
| Valid Authentication | `1234#` | Access granted, servo moves to 90° |
| Automatic Relock | Wait ~5 seconds | Servo returns to 0° |
| Failed Attempt 1 & 2 | Incorrect PIN | Access denied, remaining attempts shown |
| Failed Attempt 3 | Incorrect PIN | Security lockout triggered |
| Lockout Alarm | During lockout | Red LED flashes, buzzer alternates |
| Lockout Recovery | Wait 10 seconds | Counter resets, system returns to `LOCKED_STATE` |

---

## 📸 Screenshots

### Initial Running State
![Initial Running State](Screenshots/01_Running.png)

### Password Entry & Masking
![Password Entry](Screenshots/02_Password_Enter.png)

### Successful Authentication
![Successful Authentication](Screenshots/03_Test-1.png)

### Failed Authentication
![Failed Authentication](Screenshots/04_Test-2.png)

### Security Lockout
![Security Lockout](Screenshots/05_Test-3.png)

---

## 📁 Repository Structure

```text
Anti-Theft-Locker-Embedded-System/
│
├── README.md
├── LICENSE
│
├── arduino_code/
│   └── locker_system.ino
│
├── circuit_diagram/
│   └── pin_mapping.md
│
├── simulation/
│   ├── diagram.json
│   └── libraries.txt
│
└── Screenshots/
    ├── 01_Running.png
    ├── 02_Password_Enter.png
    ├── 03_Test-1.png
    ├── 04_Test-2.png
    └── 05_Test-3.png
```

---

## 🔐 Security Considerations

This project is an **educational embedded-system prototype**, not a production-grade security product. The prototype PIN (`1234`) is intentionally visible in the source code.

For a production implementation, the following would be needed:

- Secure credential storage instead of hard-coded credentials
- Password change functionality
- EEPROM or dedicated secure non-volatile storage
- Stronger authentication mechanisms and physical tamper detection
- Persistent, timestamped security event logging
- Encrypted communication for remote connectivity
- Protection against credential disclosure and secure key management

---

## ⚠️ Project Limitations

- Authentication credentials are stored directly in firmware
- The SG90 servo represents a locking mechanism for simulation purposes only
- No physical tamper sensor
- No persistent access log or remote notification mechanism
- Validated through Wokwi simulation rather than physical hardware deployment

---

## 🚀 Future Enhancements

1. RFID authentication (MFRC522) and dual-factor PIN + RFID
2. Administrator password management
3. EEPROM-based credential storage
4. Multiple-user authentication
5. Bluetooth (HC-05) or Wi-Fi (ESP32) connectivity
6. Cloud security notifications via MQTT
7. RTC-based access logging (DS3231) with timestamped events
8. Remote mobile/web monitoring
9. Battery operation and voltage monitoring
10. Physical tamper detection
11. Encrypted communication for remote access

---

## 📚 Skills & Learning Outcomes

This project provided hands-on experience across the full embedded-system development cycle — from pin configuration and keypad interfacing through password processing, authentication logic, and state-driven output control:

**Embedded Programming:** Embedded C/C++ · Arduino UNO / ATmega328P development · GPIO and PWM control · `millis()`-based timing

**Hardware Interfacing:** Matrix keypad scanning · I2C communication · LCD interfacing · Servo motor control · Buzzer tone generation

**Software Architecture:** Finite State Machine design · Modular firmware structure · Input validation and password masking · Failed-attempt detection and security lockout logic

**Engineering & Development:** Circuit design and pin mapping · Embedded-system debugging · Wokwi simulation and functional testing · Git version control and technical documentation

---

## 👤 Author

**Subham Bhattacherjee**
**Project:** Anti-Theft Locker Embedded System
**GitHub:** [github.com/Subhamrbj](https://github.com/Subhamrbj) · [View Repository](https://github.com/Subhamrbj/Anti-Theft-Locker-Embedded-System)
**Simulation:** https://wokwi.com/projects/472327786263779329

---

## 📜 License

This project is licensed under the **MIT License**. See the [`LICENSE`](LICENSE) file for full details.
