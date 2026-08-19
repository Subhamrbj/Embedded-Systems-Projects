# 🚗 Smart Parking Ultrasonic System — ESP32 IoT Embedded Controller

![Platform](https://img.shields.io/badge/platform-ESP32-E7352C?logo=espressif&logoColor=white)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![Simulation](https://img.shields.io/badge/simulation-Wokwi-8A2BE2)
![Connectivity](https://img.shields.io/badge/connectivity-Wi--Fi%20%2B%20HTTP-blue)
![Status](https://img.shields.io/badge/status-Completed-brightgreen)
![License](https://img.shields.io/badge/license-MIT-green)

<p align="center">

[▶️ **Run Live Wokwi Simulation**](https://wokwi.com/projects/472127403112620033)

</p>

> An ESP32-based IoT smart-parking controller that monitors four slots via ultrasonic sensors, applies statistical filtering and debounced state stabilization, drives a servo-controlled entrance gate and buzzer alert, and exposes a live, auto-refreshing web dashboard over Wi-Fi — a compact model of the sensing-to-cloud pipeline used in real smart-facility systems.

**🔗 [Live Wokwi Simulation](https://wokwi.com/projects/472127403112620033) &nbsp;|&nbsp; 📄 [sketch.ino](#) &nbsp;|&nbsp; 🖼️ [Screenshots](#-screenshots)**

---

## 💼 Why This Project Matters

This project goes well beyond a single-sensor demo — it's a **multi-node sensing system with fault tolerance, statistical filtering, debounced state logic, physical actuation, and a networked dashboard**, all coordinated by one ESP32. It demonstrates exactly the kind of engineering judgment real IoT deployments require: how to reject noisy sensor data, how to avoid flapping between states, how to fail safe when a sensor goes bad, and how to expose live system state both locally (OLED/LED) and remotely (Wi-Fi/HTTP) — without ever mis-reporting a faulty slot as available.

**At a glance:**

| | |
|---|---|
| 🎯 **Role demonstrated** | IoT / Embedded Systems Engineer — sensing, control, and connectivity |
| 🔧 **Core stack** | ESP32 · C/C++ · Wi-Fi · HTTP WebServer · I2C · Ultrasonic Sensing |
| 🧪 **Validation** | 5 occupancy scenarios + explicit sensor-fault handling, all verified |
| 📦 **Deliverables** | Firmware, circuit definition, live web dashboard, documented test evidence |

---

## 📑 Table of Contents

1. [Project Overview](#-project-overview)
2. [Objectives](#-objectives)
3. [Key Features](#-key-features)
4. [System Architecture](#️-system-architecture)
5. [System Workflow](#-system-workflow)
6. [Parking Detection Logic](#-parking-detection-logic)
7. [Sensor Filtering](#-sensor-filtering)
8. [State Stabilization](#️-state-stabilization)
9. [LED Indication](#-led-indication)
10. [Gate Control](#-gate-control)
11. [Parking-Full Alert](#-parking-full-alert)
12. [OLED Display](#️-oled-display)
13. [Wi-Fi Web Dashboard](#-wi-fi-web-dashboard)
14. [Pin Configuration](#-pin-configuration)
15. [Firmware Parameters](#️-firmware-parameters)
16. [Fail-Safe Sensor Handling](#-fail-safe-sensor-handling)
17. [Parking-State Examples](#-parking-state-examples)
18. [Technology Stack](#️-technology-stack)
19. [How to Run](#️-how-to-run-the-project)
20. [Repository Structure](#-repository-structure)
21. [Testing and Validation](#-testing-and-validation)
22. [Screenshots](#-screenshots)
23. [Roadmap](#-roadmap)
24. [Skills Demonstrated](#-skills-demonstrated)
25. [Author](#-author)
26. [License](#-license)

---

## 📌 Project Overview

The **Smart Parking Ultrasonic System** is an ESP32-based IoT embedded prototype that monitors real-time occupancy across **four parking slots**. Each slot uses an ultrasonic sensor to measure distance; the ESP32 filters the readings, classifies each slot as **FREE**, **OCCUPIED**, or **SENSOR FAULT**, and reflects that state across an OLED display, per-slot LED indicators, a servo-controlled entrance gate, a buzzer alert, structured Serial telemetry, and a live Wi-Fi web dashboard.

The full system — sensing, filtering, decision logic, actuation, and networking — was designed and functionally validated end-to-end in **Wokwi**.

---

## 🎯 Objectives

* Detect vehicle presence in individual parking slots.
* Classify each slot as FREE or OCCUPIED with high reliability.
* Reduce false state changes through sensor filtering and debounce logic.
* Display parking information locally via OLED and per-slot LEDs.
* Compute live available / occupied / faulty slot counts.
* Automatically control the entrance barrier via servo motor.
* Sound an audible alert when no safe parking space remains.
* Provide remote, real-time parking-status monitoring over Wi-Fi.
* Demonstrate a complete, fail-safe ESP32 IoT smart-parking architecture.

---

## ✨ Key Features

* Four-slot real-time parking monitoring on a single ESP32
* Four independent ultrasonic distance sensors
* 35 cm occupancy threshold with statistical outlier rejection
* Five-sample filtering with highest/lowest sample removal
* Consecutive-decision + 800 ms debounce state stabilization
* Automatic sensor-fault detection with fail-safe handling
* Per-slot green/red LED status indication
* Live OLED parking-status dashboard
* Automatic available / occupied / fault slot counting
* Servo-controlled entrance gate, driven by live availability
* Buzzer alert when parking is full
* Wi-Fi connectivity with an HTTP web dashboard
* Auto-refreshing browser dashboard (every 3 seconds)
* Structured Serial Monitor telemetry
* Fully validated in Wokwi simulation

---

## 🏗️ System Architecture

```text
                         ┌─────────────────────┐
                         │        ESP32        │
                         │   Main Controller   │
                         └──────────┬──────────┘
                                    │
              ┌─────────────────────┼─────────────────────┐
              │                     │                     │
              ▼                     ▼                     ▼
      ┌───────────────┐      ┌────────────┐      ┌──────────────┐
      │ 4 Ultrasonic  │      │    OLED    │      │     Wi-Fi    │
      │    Sensors    │      │  Display   │      │ Connectivity │
      └───────┬───────┘      └────────────┘      └──────┬───────┘
              │                                         │
              ▼                                         ▼
      ┌───────────────┐                         ┌────────────────┐
      │ Slot Occupancy│                         │ Web Dashboard  │
      │   Detection   │                         └────────────────┘
      └───────┬───────┘
              │
       ┌──────┼───────────────┐
       │      │               │
       ▼      ▼               ▼
     LEDs   Buzzer          Servo
       │      │               │
       └──────┴───────┬───────┘
                      ▼
              Parking Status
                      │
                      ▼
                Entrance Gate
```

---

## 🔄 System Workflow

```text
Ultrasonic Sensors
        ↓
Collect 5 Distance Samples
        ↓
Remove Highest Reading → Remove Lowest Reading → Average Remaining 3
        ↓
Validate Sensor Reading
        ↓
Apply 35 cm Occupancy Threshold
        ↓
Consecutive Decisions + 800 ms Debounce
        ↓
FREE / OCCUPIED / SENSOR FAULT
        ↓
┌───────────┬───────────┬───────────┬──────────────┐
│           │           │           │              │
▼           ▼           ▼           ▼              ▼
OLED       LEDs       Servo       Buzzer       Web Dashboard
```

---

## 📡 Parking Detection Logic

```text
Distance < 35 cm  →  OCCUPIED
Distance ≥ 35 cm  →  FREE
```

```cpp
const float OCCUPIED_THRESHOLD_CM = 35.0f;
```

---

## 🧮 Sensor Filtering

Each sensor uses a **five-sample statistical filter**:

```text
5 readings → Remove highest → Remove lowest → Average remaining 3 → Filtered distance
```

This trims outlier readings before they can influence a decision. The firmware also validates each sample; if **two or more of the five readings are invalid**, the sensor is flagged as faulty and its slot is excluded from the available-slot count — preventing a broken sensor from silently advertising a phantom space.

---

## ⏱️ State Stabilization

A detected state change isn't applied instantly. The firmware requires:

* At least **2 consecutive decisions** in agreement
* A minimum **800 ms debounce window**

before committing to a new FREE/OCCUPIED state — eliminating flicker from momentary sensor noise or a vehicle passing through the detection zone.

```text
New Sensor Decision → Candidate State → Consecutive Decision Check → 800 ms Stability Check → Apply New State
```

---

## 🚦 LED Indication

| Condition | Green LED | Red LED |
|---|---:|---:|
| FREE | ON | OFF |
| OCCUPIED | OFF | ON |
| SENSOR FAULT | OFF | ON |

A sensor fault deliberately shares the OCCUPIED indication — a fail-safe default that never over-reports availability.

---

## 🚧 Gate Control

```text
Available slots > 0  →  GATE OPEN
Available slots = 0  →  GATE CLOSED
```

> **Design note:** this is an availability-driven gate-control prototype. A production system would typically pair this with a dedicated entrance vehicle-detection sensor; here, parking availability itself demonstrates the automatic barrier logic.

---

## 🔊 Parking-Full Alert

```text
Available slots = 0 → Parking Full → Gate CLOSED → Buzzer Alert
```

The buzzer emits a periodic double-beep while the lot remains full.

---

## 🖥️ OLED Display

Live local view of available slots, individual slot states, occupied/free/fault counts, and gate state, using the notation:

```text
F = FREE   |   O = OCCUPIED   |   X = SENSOR FAULT
```

---

## 🌐 Wi-Fi Web Dashboard

```text
SSID: Wokwi-GUEST
Password: (empty)
```

Once connected, the ESP32 runs an HTTP server on port `80`, with the address printed to Serial:

```text
Web Dashboard: http://<ESP32-IP-ADDRESS>
```

The dashboard shows available/occupied counts, sensor-fault count, per-slot distance and status, gate state, and parking-full status — auto-refreshing every **3 seconds**, so no manual polling is needed.

---

## 🔌 Pin Configuration

| Component | GPIO Pin(s) | Purpose |
|---|---:|---|
| Slot 1 Ultrasonic | TRIG 5, ECHO 17 | Distance sensing |
| Slot 2 Ultrasonic | TRIG 16, ECHO 4 | Distance sensing |
| Slot 3 Ultrasonic | TRIG 27, ECHO 26 | Distance sensing |
| Slot 4 Ultrasonic | TRIG 25, ECHO 33 | Distance sensing |
| Slot 1 LEDs | Green 18, Red 2 | Slot indication |
| Slot 2 LEDs | Green 19, Red 12 | Slot indication |
| Slot 3 LEDs | Green 21, Red 13 | Slot indication |
| Slot 4 LEDs | Green 22, Red 14 | Slot indication |
| OLED | SDA 32, SCL 23 | I2C display |
| Servo Motor | GPIO 15 | Gate control |
| Buzzer | GPIO 0 | Parking-full alert |

> ⚠️ **Hardware note:** GPIO 0 is a boot-strapping pin on the ESP32 — its state at power-on/reset determines whether the chip boots normally or enters flashing mode. This is fine in Wokwi, but on real hardware, anything holding GPIO 0 LOW at reset (e.g. a miswired buzzer) can prevent the board from booting. For physical deployment, move the buzzer to a non-strapping GPIO.

---

## ⚙️ Firmware Parameters

| Parameter | Value |
|---|---:|
| Total Parking Slots | 4 |
| Occupied Threshold | 35 cm |
| Sensor Samples | 5 |
| Filtered Samples | 3 |
| Required Consecutive Decisions | 2 |
| State Debounce | 800 ms |
| OLED Update Interval | 500 ms |
| Serial Update Interval | 1000 ms |
| Web Dashboard Refresh | 3 seconds |
| Wi-Fi Retry Interval | 10 seconds |
| Servo Closed Angle | 0° |
| Servo Open Angle | 90° |
| Buzzer Frequency | 2000 Hz |

---

## 🧠 Fail-Safe Sensor Handling

```text
2+ Invalid Readings in a 5-Sample Group
   → Sensor Fault → Slot marked SENSOR FAULT
   → Red LED ON, Green LED OFF → Slot excluded from available count
```

This guarantees a malfunctioning sensor can never falsely present a slot as available.

---

## 📊 Parking-State Examples

| Slot 1 | Slot 2 | Slot 3 | Slot 4 | Available | Gate |
|---|---|---|---|---:|---|
| FREE | FREE | FREE | FREE | 4 | OPEN |
| OCCUPIED | FREE | FREE | FREE | 3 | OPEN |
| OCCUPIED | OCCUPIED | FREE | FREE | 2 | OPEN |
| OCCUPIED | OCCUPIED | OCCUPIED | FREE | 1 | OPEN |
| OCCUPIED | OCCUPIED | OCCUPIED | OCCUPIED | 0 | CLOSED |

A slot with a sensor fault is always excluded from the available count.

---

## 🛠️ Technology Stack

**Hardware / Embedded:** ESP32 · 4 × Ultrasonic Sensors · SSD1306 OLED · Green/Red LEDs · Servo Motor · Buzzer

**Software:** C/C++ · Arduino framework · ESP32 Wi-Fi · HTTP WebServer · I2C · Wokwi

**Libraries:** Adafruit GFX Library · Adafruit SSD1306

---

## ▶️ How to Run the Project

### 1. Open the Live Simulation
**[▶️ Open Live Wokwi Simulation](https://wokwi.com/projects/472127403112620033)** — includes the firmware, circuit (`diagram.json`), and required libraries (`libraries.txt`).

### 2. Start the Simulation
Click **▶ Start Simulation**. The system initializes ultrasonic sensors, LEDs, OLED, servo, buzzer, Wi-Fi, and the web server.

### 3. Observe the Serial Monitor
Streams slot distance, slot state, occupied/fault/available counts, gate state, and the Wi-Fi IP address in real time.

### 4. Open the Web Dashboard
Copy the IP address printed to Serial and open it in a browser:

```text
http://<ESP32-IP-ADDRESS>
```

Live parking status, refreshing automatically every 3 seconds.

---

## 📂 Repository Structure

```text
Smart-Parking-Ultrasonic-Embedded-System/
│
├── Output/
│   ├── 01_all_slots_free.png
│   ├── 02_one_slot_occupied.png
│   ├── 03_two_slots_occupied.png
│   ├── 04_three_slots_occupied.png
│   └── 05_parking_full.png
│
├── sketch.ino
├── diagram.json
├── libraries.txt
└── README.md
```

---

## 🧪 Testing and Validation

| Test Case | Available | Occupied | Gate | Buzzer |
|---|---:|---:|---|---|
| All Slots Free | 4 | 0 | OPEN | OFF |
| One Slot Occupied | 3 | 1 | OPEN | OFF |
| Two Slots Occupied | 2 | 2 | OPEN | OFF |
| Three Slots Occupied | 1 | 3 | OPEN | OFF |
| Parking Full | 0 | 4 | CLOSED | ACTIVE |
| Sensor Fault | — | — | Faulty slot excluded; Red LED ON, Green LED OFF | — |

**6 / 6 scenarios verified**, including explicit fail-safe fault handling — not just the happy path.

---

## 📸 Screenshots

### All Slots Free
![All Slots Free](Output/01_all_slots_free.png)
`Available: 4 | Occupied: 0 | Gate: OPEN`

### One Slot Occupied
![One Slot Occupied](Output/02_one_slot_occupied.png)
`Available: 3 | Occupied: 1 | Gate: OPEN`

### Two Slots Occupied
![Two Slots Occupied](Output/03_two_slots_occupied.png)
`Available: 2 | Occupied: 2 | Gate: OPEN`

### Three Slots Occupied
![Three Slots Occupied](Output/04_three_slots_occupied.png)
`Available: 1 | Occupied: 3 | Gate: OPEN`

### Parking Full
![Parking Full](Output/05_parking_full.png)
`Available: 0 | Occupied: 4 | Gate: CLOSED | Buzzer: ACTIVE`

---

## 🚀 Roadmap

| Area | Planned Enhancements |
|---|---|
| 🚧 **Access Control** | Dedicated entrance/exit vehicle-detection sensors, RFID identification, QR-code access |
| ☁️ **Connectivity** | Cloud data storage, MQTT-based IoT communication, mobile app integration |
| 📈 **Operations** | Parking reservation, historical occupancy analytics, larger-lot scalability |
| 🛠️ **Reliability** | Advanced sensor diagnostics, OTA firmware updates |
| 🔐 **Security** | Web dashboard authentication, HTTPS-secured communication |
| 🏗️ **Deployment** | Real-world hardware validation |

---

## 💼 Skills Demonstrated

Embedded C/C++ · ESP32 programming · IoT system development · ultrasonic sensor interfacing · distance measurement · statistical sensor filtering · debounced state-machine logic · fault detection & fail-safe design · GPIO control · servo/PWM control · I2C communication · OLED programming · Wi-Fi networking · HTTP web-server development · real-time monitoring · Wokwi simulation · Serial debugging · Git/GitHub · technical documentation.

---

## 👤 Author

**Subham Bhattacherjee**
**Project:** Smart Parking Ultrasonic Embedded System
**GitHub:** [github.com/Subhamrbj/Smart-Parking-Ultrasonic-Embedded-System](https://github.com/Subhamrbj/Smart-Parking-Ultrasonic-Embedded-System)
**Live Simulation:** [wokwi.com/projects/472127403112620033](https://wokwi.com/projects/472127403112620033)

---

## 📜 License

Licensed under the **MIT License** — free to use, modify, and distribute for personal, academic, or commercial purposes, with attribution appreciated.

---

## ⭐ Project Summary

**Highlights for recruiters:**

* Built a **four-node IoT sensing system** on a single ESP32 with real-time occupancy classification.
* Implemented **statistical outlier-rejection filtering** and **debounced, consecutive-decision state stabilization** to eliminate false transitions.
* Designed **fail-safe fault handling** — a malfunctioning sensor is detected and excluded from availability counts rather than silently trusted.
* Drove **physical actuation** (servo gate, buzzer) directly from computed system state.
* Built and served a **live, auto-refreshing HTTP web dashboard** over Wi-Fi from the ESP32 itself.
* Verified **6 distinct scenarios**, including edge-case sensor-fault handling, with documented evidence.
* Structured for reproducibility: `sketch.ino`, `diagram.json`, `libraries.txt`, `Output/`, and a public Wokwi simulation link.

> **Project Type:** IoT / Embedded Systems / Smart Infrastructure &nbsp;·&nbsp; **Platform:** ESP32 &nbsp;·&nbsp; **Language:** Embedded C/C++ &nbsp;·&nbsp; **Simulation:** Wokwi &nbsp;·&nbsp; **Connectivity:** Wi-Fi + HTTP
