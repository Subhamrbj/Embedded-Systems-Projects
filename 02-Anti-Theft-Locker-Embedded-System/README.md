# 🔒 Anti-Theft Locker Embedded System

> **An Arduino UNO-based electronic access-control system with PIN authentication, 4×4 keypad input, masked password display, SG90 servo locking, automatic relocking, failed-attempt detection, and security lockout alarm.**

<p align="center">

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-Embedded%20C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![Simulation](https://img.shields.io/badge/Simulation-Wokwi-8A2BE2)
![Architecture](https://img.shields.io/badge/Architecture-Finite%20State%20Machine-6F42C1)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)
![License](https://img.shields.io/badge/License-MIT-green)

</p>

<p align="center">

[▶️ **Run Live Wokwi Simulation**](https://wokwi.com/projects/472327786263779329)

</p>

---

## 📌 Overview

The **Anti-Theft Locker Embedded System** is an Arduino UNO-based electronic access-control prototype designed to demonstrate how authentication, hardware interfacing, actuator control, timing logic, and security mechanisms can be integrated into a complete embedded system.

The system uses a **4×4 matrix keypad** for PIN entry, a **16×2 I2C LCD** for user feedback, an **SG90 servo motor** to simulate the physical locking mechanism, Green and Red LEDs for status indication, and a piezo buzzer for audible feedback and security alarms.

The firmware is structured around a **Finite State Machine (FSM)** with three primary states:

- `LOCKED_STATE`
- `UNLOCKED_STATE`
- `LOCKOUT_STATE`

A correct PIN temporarily unlocks the system. After approximately **5 seconds**, the locker automatically relocks. Three consecutive incorrect authentication attempts trigger a **10-second security lockout**, during which keypad input is blocked and an alternating alarm is activated.

The complete prototype has been implemented and functionally tested in the **Wokwi electronics simulator**.

---

## 🎯 Project Objectives

- Implement PIN-based electronic locker authentication.
- Interface a 4×4 matrix keypad with Arduino UNO.
- Mask password input on a 16×2 I2C LCD.
- Control an SG90 servo as a simulated locking mechanism.
- Implement a finite state machine for system control.
- Automatically relock the locker after successful authentication.
- Detect consecutive failed authentication attempts.
- Trigger a three-attempt security lockout.
- Provide visual status feedback using LEDs.
- Provide audible feedback using a piezo buzzer.
- Display system information through the Serial Monitor.
- Validate the complete embedded system through simulation.

---

## ✨ Key Features

- 🔐 PIN-based authentication
- 🔢 4×4 matrix keypad
- 📺 16×2 I2C LCD
- 🔒 SG90 servo-based locking mechanism
- 🔓 Servo unlock position: **90°**
- 🔒 Servo lock position: **0°**
- 🔢 Password masking using `*`
- 🧹 Password clearing using `*`
- ✅ Password submission using `#`
- ⏱️ Approximately 5-second automatic relocking
- 🚨 Three-strike failed-attempt detection
- 🔐 10-second security lockout
- 🔴 Flashing Red LED during lockout
- 🟢 Green LED during successful unlock
- 🔊 2000 Hz keypad feedback tone
- 🔊 Access-granted confirmation tones
- 🔊 400 Hz access-denied warning tone
- 🚨 Alternating 1200 Hz / 800 Hz security alarm
- 📡 9600 baud Serial Monitor telemetry
- 🧠 Finite State Machine architecture
- 🧪 Wokwi simulation and functional testing

---

# 🏗️ System Architecture

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
     ┌─────────────────────┐
     │ Finite State Machine │
     │                      │
     │ LOCKED               │
     │ UNLOCKED             │
     │ LOCKOUT              │
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

# 🔄 System Workflow & Finite State Machine

```text
                         ┌──────────────────────┐
                         │     LOCKED_STATE     │
                         │ Servo = 0°           │
                         │ Red LED = ON         │
                         └──────────┬───────────┘
                                    │
                               Enter PIN
                                    │
                               Press '#'
                                    │
                         ┌──────────▼───────────┐
                         │   PIN VALIDATION     │
                         └───────┬────────┬─────┘
                                 │        │
                            VALID│        │INVALID
                                 │        │
                                 ▼        ▼
                    ┌────────────────┐  ┌──────────────────┐
                    │ UNLOCKED_STATE │  │  ACCESS DENIED   │
                    │ Servo = 90°    │  │ Fail Count + 1   │
                    │ Green LED ON   │  └────────┬─────────┘
                    └───────┬────────┘           │
                            │                    │
                         ~5 sec             Attempts < 3
                            │                    │
                            ▼                    ▼
                     LOCKED_STATE          TRY AGAIN
                                                 │
                                                 │
                                           Attempts >= 3
                                                 │
                                                 ▼
                                      ┌────────────────────┐
                                      │   LOCKOUT_STATE    │
                                      │ Servo = 0°         │
                                      │ Keypad blocked     │
                                      │ Alarm active       │
                                      │ Red LED flashing   │
                                      └─────────┬──────────┘
                                                │
                                             10 sec
                                                │
                                                ▼
                                          Reset Counter
                                                │
                                                ▼
                                         LOCKED_STATE
```

---

# 🔐 Authentication Logic

## Default Prototype PIN

```text
1234
```

> **Note:** The PIN is intentionally visible because this is an educational prototype. It should not be treated as a production security implementation.

## Successful Authentication

```text
User enters PIN
       │
       ▼
Press '#'
       │
       ▼
PIN == 1234 ?
       │
       ├──────── YES ────────► ACCESS GRANTED
       │                           │
       │                           ├── Servo → 90°
       │                           ├── Green LED → ON
       │                           ├── Red LED → OFF
       │                           ├── Confirmation tone
       │                           └── Start auto-lock timer
       │
       └──────── NO ─────────► ACCESS DENIED
```

## Failed Authentication

```text
Incorrect PIN
      │
      ▼
Fail Count + 1
      │
      ▼
Attempts >= 3 ?
      │
      ├── NO ──► ACCESS DENIED
      │             │
      │             ├── Warning tone
      │             ├── Remaining attempts displayed
      │             └── Return to password entry
      │
      └── YES ─► LOCKOUT_STATE
                    │
                    ├── Keypad input blocked
                    ├── Servo remains locked
                    ├── Red LED alternates
                    ├── Alarm: 1200 / 800 Hz
                    └── 10-second lockout
                              │
                              ▼
                         Counter reset
                              │
                              ▼
                         LOCKED_STATE
```

---

# 🔓 Servo Locking Mechanism

The **SG90 servo motor** simulates the physical locker deadbolt.

| System State | Servo Position | Meaning |
|---|---:|---|
| `LOCKED_STATE` | 0° | Locker locked |
| `UNLOCKED_STATE` | 90° | Locker unlocked |
| `LOCKOUT_STATE` | 0° | Locker remains locked |

### Firmware Configuration

```cpp
const byte LOCK_ANGLE = 0;
const byte UNLOCK_ANGLE = 90;
```

---

# ⏱️ Automatic Relocking

After successful authentication, the firmware enters `UNLOCKED_STATE` and records the current time using `millis()`.

```cpp
const unsigned long AUTO_LOCK_DELAY = 5000UL;
```

### Operation

```text
ACCESS GRANTED
      │
      ▼
Servo → 90°
Green LED → ON
      │
      ▼
Start millis()-based timer
      │
      ▼
Approximately 5 seconds
      │
      ▼
Servo → 0°
Red LED → ON
      │
      ▼
LOCKED_STATE
```

The main automatic relock timer is implemented using `millis()` rather than a five-second blocking delay.

> **Implementation note:** Short `delay()` calls are still used in some UI and buzzer feedback routines. The primary relock and lockout timing are handled using `millis()`.

---

# 🚨 Failed-Attempt Detection & Security Lockout

The security configuration is:

```cpp
const byte MAX_FAILED_ATTEMPTS = 3;
const unsigned long LOCKOUT_TIME = 10000UL;
```

After the third failed authentication attempt, the system enters `LOCKOUT_STATE`.

### Lockout Behavior

- 🔒 Servo remains at 0°.
- 🔢 Keypad input is blocked.
- 🔴 Red LED alternates every 250 ms.
- 🟢 Green LED remains OFF.
- 🔊 Buzzer alternates between 1200 Hz and 800 Hz.
- 📺 LCD displays `TOO MANY` / `ATTEMPTS!`.
- ⏱️ Lockout lasts 10 seconds.
- 🔄 Failed-attempt counter is reset.
- 🔐 System returns to `LOCKED_STATE`.

The alarm and LED alternation are controlled using a `millis()`-based 250 ms timer.

---

# 🚦 LED & Buzzer Status

| System Condition | Green LED | Red LED | Buzzer |
|---|---|---|---|
| `LOCKED_STATE` | OFF | ON | Silent |
| Keypress | State dependent | State dependent | 2000 Hz / 50 ms |
| `UNLOCKED_STATE` | ON | OFF | 1000 Hz → 1500 Hz |
| `ACCESS DENIED` | OFF | ON | 400 Hz / 500 ms |
| `LOCKOUT_STATE` | OFF | Alternating | 1200 Hz ↔ 800 Hz |

---

# 📺 LCD User Interface

The 16×2 I2C LCD provides real-time system feedback.

### Standby

```text
ENTER PASSWORD:
PASS:****
```

### Access Granted

```text
ACCESS GRANTED
LOCKER OPEN
```

### Access Denied

```text
ACCESS DENIED
TRIES LEFT: 2
```

### Security Lockout

```text
TOO MANY
ATTEMPTS!
```

### Automatic Relock

```text
LOCKER LOCKED
SECURE
```

### Lockout Recovery

```text
LOCKOUT ENDED
TRY AGAIN
```

---

# 🔢 Keypad Operation

The 4×4 keypad is configured as:

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

The password input is limited to the configured maximum length of **8 characters**.

---

# 🔌 Pin Configuration

| Component | Module Pin | Arduino UNO Pin | Function |
|---|---|---|---|
| 4×4 Matrix Keypad | Row 1 | D9 | Matrix row |
| 4×4 Matrix Keypad | Row 2 | D8 | Matrix row |
| 4×4 Matrix Keypad | Row 3 | D7 | Matrix row |
| 4×4 Matrix Keypad | Row 4 | D6 | Matrix row |
| 4×4 Matrix Keypad | Column 1 | D5 | Matrix column |
| 4×4 Matrix Keypad | Column 2 | D4 | Matrix column |
| 4×4 Matrix Keypad | Column 3 | D3 | Matrix column |
| 4×4 Matrix Keypad | Column 4 | D2 | Matrix column |
| I2C LCD | SDA | A4 | I2C data |
| I2C LCD | SCL | A5 | I2C clock |
| I2C LCD | VCC / GND | 5V / GND | Power |
| SG90 Servo | Signal | D10 | PWM control |
| SG90 Servo | VCC / GND | 5V / GND | Power |
| Piezo Buzzer | Positive | D11 | Audio output |
| Piezo Buzzer | GND | GND | Common ground |
| Green LED | Anode | D12 via 220Ω | Unlock indicator |
| Red LED | Anode | D13 via 220Ω | Lock / alarm indicator |

---

# ⚙️ Firmware Parameters

| Parameter | Value | Description |
|---|---|---|
| Default PIN | `1234` | Prototype access PIN |
| Maximum Password Length | 8 characters | Maximum accepted input |
| Lock Angle | 0° | Locked position |
| Unlock Angle | 90° | Unlocked position |
| Auto-Lock Delay | 5000 ms | Automatic relock interval |
| Maximum Failed Attempts | 3 | Attempts before lockout |
| Lockout Duration | 10000 ms | Security lockout duration |
| Alarm Frequency 1 | 1200 Hz | High alarm tone |
| Alarm Frequency 2 | 800 Hz | Low alarm tone |
| Alarm Toggle Rate | 250 ms | Alarm/LED alternation |
| Keypress Tone | 2000 Hz / 50 ms | Key feedback |
| Access Denied Tone | 400 Hz / 500 ms | Warning tone |
| Access Granted Tone | 1000 Hz → 1500 Hz | Success feedback |
| UART Baud Rate | 9600 | Serial communication |
| LCD I2C Address | `0x27` | LCD address |
| LCD Size | 16×2 | Display size |

---

# 🧠 Firmware Architecture

The firmware uses a **Finite State Machine** to separate the major operating conditions of the locker.

## System States

```cpp
enum SystemState {
    LOCKED_STATE,
    UNLOCKED_STATE,
    LOCKOUT_STATE
};
```

## State Responsibilities

| State | Responsibility |
|---|---|
| `LOCKED_STATE` | Normal secure standby and password authentication |
| `UNLOCKED_STATE` | Temporary authorized access |
| `LOCKOUT_STATE` | Security response after three failed attempts |

## Major Firmware Functions

```text
initializeHardware()
showWelcomeScreen()
showPasswordScreen()

processKey()
handleDigit()
handleSubmit()
handleClear()

checkPassword()
grantAccess()
lockLocker()
denyAccess()

triggerLockout()
handleLockout()

resetPasswordInput()
updatePasswordDisplay()

setLockedIndicators()
setUnlockedIndicators()
setAlarmIndicators()
stopAlarm()

beepKey()
beepAccessGranted()
beepAccessDenied()

printSystemStatus()
```

---

# 🛠️ Technology Stack

## Hardware

- Arduino UNO
- ATmega328P
- 4×4 Matrix Keypad
- SG90 Servo Motor
- 16×2 I2C LCD
- PCF8574 I2C LCD interface
- Piezoelectric Buzzer
- Green LED
- Red LED
- 220Ω Resistors

## Software & Libraries

- Embedded C/C++
- Arduino Core
- `Keypad.h`
- `Servo.h`
- `Wire.h`
- `LiquidCrystal_I2C.h`
- Wokwi Electronics Simulator
- Git
- GitHub

---

# ▶️ How to Run

## 1. Open the Wokwi Simulation

Launch the complete simulation:

**[▶️ Open Live Wokwi Simulation](https://wokwi.com/projects/472327786263779329)**

The simulation contains the Arduino firmware, circuit configuration, and required libraries.

## 2. Start the Simulation

Click:

```text
▶ Start Simulation
```

The system initializes:

- Arduino UNO
- 4×4 keypad
- 16×2 I2C LCD
- SG90 servo
- Piezo buzzer
- Green LED
- Red LED
- Serial Monitor

## 3. Test Successful Authentication

Enter:

```text
1234#
```

Expected sequence:

```text
Valid PIN
   ↓
ACCESS GRANTED
   ↓
Servo → 90°
Green LED → ON
   ↓
Approximately 5-second timer
   ↓
Servo → 0°
Red LED → ON
   ↓
LOCKED_STATE
```

## 4. Test Failed Authentication

Enter an incorrect PIN, for example:

```text
9999#
```

Expected result:

```text
ACCESS DENIED
TRIES LEFT: 2
```

Enter another incorrect PIN:

```text
9999#
```

Expected result:

```text
ACCESS DENIED
TRIES LEFT: 1
```

## 5. Test Security Lockout

Enter an incorrect PIN for the third time:

```text
9999#
```

Expected result:

```text
TOO MANY
ATTEMPTS!
```

During the lockout:

- Keypad input is blocked.
- Servo remains locked.
- Red LED alternates.
- Alarm alternates between 1200 Hz and 800 Hz.
- Lockout lasts 10 seconds.
- Failed-attempt counter resets.
- System returns to `LOCKED_STATE`.

---

# 🧪 Testing & Validation

The project was tested in Wokwi using multiple functional scenarios.

| Test Case | Input / Condition | Expected Result |
|---|---|---|
| Initial Boot | Start simulation | `LOCKED_STATE`, Red LED ON, servo at 0° |
| Password Entry | Enter numeric PIN | LCD displays masked `*` characters |
| Clear Input | Press `*` | Password input cleared |
| Valid Authentication | `1234#` | Access granted, servo moves to 90° |
| Automatic Relock | Wait approximately 5 seconds | Servo returns to 0° |
| Failed Attempt 1 | Incorrect PIN | Access denied, 2 attempts remaining |
| Failed Attempt 2 | Incorrect PIN | Access denied, 1 attempt remaining |
| Failed Attempt 3 | Incorrect PIN | Security lockout triggered |
| Lockout Alarm | During lockout | Red LED flashes and buzzer alternates |
| Lockout Recovery | Wait 10 seconds | Counter resets and system returns to locked state |

---

# 📊 End-to-End Test Flow

```text
                    ┌──────────────────┐
                    │   INITIAL BOOT   │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │   LOCKED_STATE   │
                    └────────┬─────────┘
                             │
                         Enter PIN
                             │
                             ▼
                    ┌──────────────────┐
                    │  AUTHENTICATION  │
                    └───────┬──┬───────┘
                            │  │
                       VALID│  │INVALID
                            │  │
                            ▼  ▼
                    ┌────────┐ ┌──────────────┐
                    │UNLOCKED│ │ FAIL COUNT+1 │
                    └───┬────┘ └──────┬───────┘
                        │              │
                    ~5 seconds     Attempts < 3
                        │              │
                        ▼              ▼
                     LOCKED       TRY AGAIN
                                       │
                                       │
                                 Attempts >= 3
                                       │
                                       ▼
                                  LOCKOUT
                                       │
                                    10 sec
                                       │
                                       ▼
                                   RESET
                                       │
                                       ▼
                                    LOCKED
```

---

# 📸 Project Screenshots

The repository contains visual evidence captured during project testing.

### 🔄 Initial Running State

![Initial Running State](Screenshots/01_Running.png)

### 🔐 Password Entry & Masking

![Password Entry](Screenshots/02_Password_Enter.png)

### 🔓 Successful Authentication

![Successful Authentication](Screenshots/03_Test-1.png)

### ⚠️ Failed Authentication

![Failed Authentication](Screenshots/04_Test-2.png)

### 🚨 Security Lockout

![Security Lockout](Screenshots/05_Test-3.png)

---

# 📁 Repository Structure

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

# 🔗 Project Resources

## 💻 Source Code & Documentation

The complete project repository contains:

- Arduino firmware
- Circuit/pin documentation
- Wokwi simulation files
- Testing screenshots
- Technical documentation
- MIT License

**[View GitHub Repository](https://github.com/Subhamrbj/Anti-Theft-Locker-Embedded-System)**

## 🧪 Live Simulation

**[▶️ Run the Project on Wokwi](https://wokwi.com/projects/472327786263779329)**

---

# 🚀 Future Enhancements

The current prototype can be extended into a more advanced access-control system through:

1. **RFID Authentication** using MFRC522.
2. **Dual-Factor Authentication** combining PIN + RFID.
3. **Administrator Password Management**.
4. **EEPROM-Based Credential Storage**.
5. **Multiple-User Authentication**.
6. **Bluetooth Access Control** using HC-05.
7. **Wi-Fi Connectivity** using ESP32.
8. **Cloud Security Notifications** using MQTT.
9. **RTC-Based Access Logging** using DS3231.
10. **Timestamped Security Event Logging**.
11. **Remote Mobile/Web Monitoring**.
12. **Battery Operation and Voltage Monitoring**.
13. **Physical Tamper Detection**.
14. **Encrypted communication for remote access**.

---

# 🔐 Security Considerations

This project is an **educational embedded-system prototype**, not a production-grade security product.

The prototype PIN is intentionally visible in the source code:

```text
1234
```

For a production implementation, the following improvements would be appropriate:

- Secure credential storage instead of hard-coded credentials.
- Password change functionality.
- EEPROM or dedicated secure non-volatile storage.
- Stronger authentication mechanisms.
- Physical tamper detection.
- Persistent security event logging.
- Encrypted communication for remote connectivity.
- Protection against credential disclosure.
- Secure key management.

---

# ⚠️ Project Limitations

The current implementation has several prototype-level limitations:

- Authentication credentials are stored directly in firmware.
- The SG90 servo represents a locking mechanism for simulation purposes.
- The system has no physical tamper sensor.
- No persistent access log is currently implemented.
- No remote notification mechanism is currently implemented.
- The project is validated through Wokwi simulation rather than physical hardware deployment.

These limitations also define clear directions for future development.

---

# 💼 Technical Skills Demonstrated

This project demonstrates practical implementation of:

### Embedded Programming

- Embedded C/C++
- Arduino UNO programming
- ATmega328P development
- GPIO control
- PWM control
- Timing with `millis()`

### Hardware Interfacing

- Matrix keypad scanning
- I2C communication
- LCD interfacing
- Servo motor control
- Buzzer tone generation
- LED control

### Software Architecture

- Finite State Machine design
- Modular firmware structure
- Input validation
- Password masking
- Authentication logic
- Failed-attempt detection
- Security lockout logic

### Engineering & Development

- Circuit design
- Pin mapping
- Embedded-system debugging
- Wokwi simulation
- Functional testing
- Git version control
- GitHub repository organization
- Technical documentation

---

# 📚 Learning Outcomes

The project provided hands-on experience with the complete embedded-system development cycle:

```text
        HARDWARE DESIGN
              │
              ▼
       PIN CONFIGURATION
              │
              ▼
       KEYPAD INTERFACING
              │
              ▼
      PASSWORD PROCESSING
              │
              ▼
       AUTHENTICATION LOGIC
              │
       ┌──────┴──────┐
       │             │
       ▼             ▼
    VALID PIN    INVALID PIN
       │             │
       ▼             ▼
    UNLOCK       FAIL COUNT
       │             │
       ▼             ▼
   SERVO 90°    ATTEMPTS >= 3
       │             │
       ▼             ▼
   ~5s TIMER     LOCKOUT
       │             │
       ▼             ▼
   SERVO 0°      10s ALARM
       │             │
       └──────┬──────┘
              ▼
        LOCKED_STATE
```

---

# 🏆 Why This Project Matters

This project goes beyond simply controlling a servo with an Arduino.

It demonstrates the integration of:

```text
┌───────────────────────────────────────────┐
│           EMBEDDED SYSTEM DESIGN          │
├───────────────────────────────────────────┤
│                                           │
│  Keypad Input                             │
│       +                                   │
│  Authentication                           │
│       +                                   │
│  Finite State Machine                     │
│       +                                   │
│  Servo Actuation                          │
│       +                                   │
│  LCD Interface                            │
│       +                                   │
│  LED & Buzzer Feedback                    │
│       +                                   │
│  Automatic Timing                         │
│       +                                   │
│  Failed-Attempt Detection                 │
│       +                                   │
│  Security Lockout                         │
│       +                                   │
│  Simulation & Testing                     │
│                                           │
└───────────────────────────────────────────┘
```

The result is a complete, documented **embedded access-control prototype** rather than an isolated hardware demonstration.

---

# 👤 Author

## Subham Bhattacherjee

**Project:** Anti-Theft Locker Embedded System

**GitHub:**  
[Subhamrbj](https://github.com/Subhamrbj)

---

# 📜 License

This project is licensed under the **MIT License**.

See the [`LICENSE`](LICENSE) file for complete license information.

---

# ⭐ Project Summary

The **Anti-Theft Locker Embedded System** demonstrates how an Arduino-based embedded controller can integrate authentication, hardware interfacing, actuator control, state-machine architecture, timing logic, user feedback, and security mechanisms into one cohesive system.

```text
Keypad Authentication
        +
LCD User Interface
        +
Servo Actuation
        +
LED Status Indication
        +
Audible Feedback
        +
Finite State Machine
        +
Automatic Relocking
        +
Failed-Attempt Detection
        +
Security Lockout
        =
Complete Embedded Locker Security Prototype
```

<p align="center">

### 🔒 Authenticate • Protect • Automate

**Built with Arduino UNO • Embedded C++ • Wokwi**

</p>
