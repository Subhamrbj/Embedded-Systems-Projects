# 🗑️ Smart Dustbin — Touchless Waste Monitoring Embedded System

![Platform](https://img.shields.io/badge/platform-Arduino%20Uno-00979D?logo=arduino\&logoColor=white)
![Simulation](https://img.shields.io/badge/simulated%20on-Wokwi-1DA1F2)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue?logo=c%2B%2B)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-Prototype-yellow)

<p align="center">

[🔗 **Run Live Wokwi Simulation**](https://wokwi.com/projects/472357743514021889)

</p>

> An Arduino-based touchless waste-bin controller that automatically opens its lid when a hand is detected, continuously monitors the bin fill level using a dedicated ultrasonic sensor, and activates visual and audible alerts when the bin reaches a configurable threshold — implemented using a fully non-blocking control architecture with `millis()`-based timing.

**📄 [sketch.ino](sketch.ino)  |  🖼️ [Output](#-output)**

---

## 💼 Why This Project Matters

This project goes beyond a basic automatic dustbin demonstration by implementing a **multi-sensor embedded control system with non-blocking firmware, independent control loops, real-time fill estimation, actuator control, visual feedback, audible alerts, and fault-aware sensor handling**.

The entire control loop is built around **`millis()`-based timing rather than `delay()`**, allowing the hand-detection sensor, bin-level sensor, servo, LCD, LEDs, and buzzer to operate concurrently without blocking one another.

The project also applies explicit range clamping to prevent invalid ultrasonic readings from generating impossible fill percentages, while documenting practical limitations such as single-point sensing and the absence of weight measurement.

**At a glance:**

|                          |                                                                    |
| ------------------------ | ------------------------------------------------------------------ |
| 🎯 **Role demonstrated** | Embedded Firmware Engineer — sensor fusion & non-blocking control  |
| 🔧 **Core stack**        | Arduino UNO · C/C++ · Dual Ultrasonic Sensing · I2C · PWM          |
| 🧪 **Validation**        | 7 test cases including concurrency and fault handling — all passed |
| 📦 **Deliverables**      | Firmware, circuit definition, telemetry, documented test evidence  |

---

## 📑 Table of Contents

1. [Project Overview](#-project-overview)
2. [Problem Statement](#-problem-statement)
3. [Industry Relevance](#-industry-relevance)
4. [Objectives](#-objectives)
5. [Key Features](#-key-features)
6. [Components Used](#-components-used)
7. [Embedded Concepts Applied](#-embedded-concepts-applied)
8. [System Architecture](#️-system-architecture)
9. [System Workflow](#-system-workflow)
10. [Circuit Connections](#-circuit-connections)
11. [Firmware Logic](#-firmware-logic)
12. [Bin-Level Calculation](#-bin-level-calculation)
13. [Alert System](#-alert-system)
14. [LCD Display](#-lcd-display)
15. [Folder Structure](#-folder-structure)
16. [Technology Stack](#️-technology-stack)
17. [How to Run](#️-how-to-run)
18. [Simulation Steps](#️-simulation-steps)
19. [Testing and Validation](#-testing-and-validation)
20. [Output](#-output)
21. [Known Limitations](#️-known-limitations)
22. [Roadmap](#-roadmap)
23. [Learning Outcomes](#-learning-outcomes)
24. [Skills Demonstrated](#-skills-demonstrated)
25. [Author](#-author)
26. [License](#-license)

---

## 📌 Project Overview

The **Smart Dustbin — Touchless Waste Monitoring Embedded System** is an Arduino Uno-based embedded prototype designed to provide both **contactless lid operation** and **real-time waste-level monitoring**.

The system uses **two independent HC-SR04 ultrasonic sensors**:

* One sensor detects a hand or object near the lid.
* The second sensor measures the distance between the sensor and the waste.
* The measured distance is converted into a **0–100% fill percentage**.
* An SG90 servo automatically opens and closes the lid.
* A 16×2 I2C LCD displays the current lid state and fill percentage.
* Green and red LEDs provide visual status indication.
* A buzzer activates when the bin reaches the configured full threshold.
* Serial telemetry provides real-time system information.

The complete system was designed and functionally validated in **Wokwi simulation**.

---

## 🎯 Objectives

* Implement touchless automatic lid opening using ultrasonic sensing.
* Detect hand/object presence within a configurable distance.
* Automatically close the lid after the hand is removed.
* Continuously monitor the waste fill level.
* Convert ultrasonic distance measurements into a 0–100% fill percentage.
* Prevent invalid percentages through sensor-range clamping.
* Display live bin information through an I2C LCD.
* Provide visual status using green and red LEDs.
* Activate an audible alert when the bin becomes full.
* Maintain concurrent operation without blocking `delay()` calls.
* Demonstrate a practical embedded smart-waste-management architecture.

---

## 🧠 Problem Statement

Traditional waste bins create several practical problems:

1. Users must physically touch the lid, increasing the possibility of contamination.
2. There is no real-time indication of the current fill level.
3. Overflow may remain unnoticed until someone manually checks the bin.
4. Fixed collection schedules may result in unnecessary collection trips.

This project addresses these issues through **touchless operation and continuous fill-level monitoring**, providing the foundation for a future IoT-enabled smart waste-management system.

---

## 🏭 Industry Relevance

| Domain                                      | Application in this Project                                                                  |
| ------------------------------------------- | -------------------------------------------------------------------------------------------- |
| **Smart City / Municipal Waste Management** | Enables need-based waste collection instead of fixed schedules                               |
| **Touchless Public Infrastructure**         | Provides hygienic, contactless interaction                                                   |
| **Industrial IoT Monitoring**               | Demonstrates the same sensing and threshold-alert pattern used in tank and silo monitoring   |
| **Embedded Systems Engineering**            | Demonstrates non-blocking firmware, sensor calibration, state handling, and actuator control |

---

## ✨ Key Features

* 🖐️ Touchless automatic lid opening using ultrasonic hand detection
* ⏱️ Automatic lid closing after a configurable hold period
* 📊 Real-time 0–100% bin fill-level estimation
* 📡 Dedicated ultrasonic sensor for waste-level measurement
* 🚨 Configurable full-bin threshold — default **85%**
* 🟢 Green LED for normal operation
* 🔴 Blinking red LED when the bin is full
* 🔊 Buzzer alert for full-bin condition
* 🖥️ 16×2 I2C LCD for live system status
* 📈 Real-time Serial telemetry
* ⚙️ Fully non-blocking `loop()` architecture
* 🔄 Independent hand-detection and fill-monitoring control loops
* 🛡️ Out-of-range reading protection through value clamping
* 🧪 7/7 functional test cases successfully verified in simulation

---

## 🔧 Components Used

| Component                 | Qty | Purpose                            |
| ------------------------- | --: | ---------------------------------- |
| Arduino Uno               |   1 | Main microcontroller               |
| HC-SR04 Ultrasonic Sensor |   2 | Hand detection + bin-level sensing |
| SG90 Servo Motor          |   1 | Automatic lid actuation            |
| 16×2 I2C LCD              |   1 | Live status display                |
| Buzzer                    |   1 | Full-bin audible alert             |
| Green LED                 |   1 | Normal status indication           |
| Red LED                   |   1 | Full-bin alert indication          |
| 220 Ω Resistor            |   2 | LED current limiting               |
| Jumper Wires / Breadboard |   — | Circuit connections                |

---

## 🧩 Embedded Concepts Applied

`Ultrasonic Ranging` · `Sensor Fusion` · `Non-Blocking Timing` · `millis()` · `State Machines` · `Signal Conditioning` · `Range Clamping` · `Analog-to-Percentage Mapping` · `I2C Communication` · `PWM Actuation` · `Serial Telemetry`

---

## 🏗️ System Architecture

```text
                         ┌───────────────────────────┐
                         │        Arduino Uno        │
                         │       Main Controller     │
                         └─────────────┬─────────────┘
                                       │
                 ┌─────────────────────┴─────────────────────┐
                 │                                           │
                 ▼                                           ▼
        ┌─────────────────┐                         ┌─────────────────┐
        │   HC-SR04 #1    │                         │   HC-SR04 #2    │
        │  Hand Detection │                         │   Waste Level   │
        └────────┬────────┘                         └────────┬────────┘
                 │                                           │
                 ▼                                           ▼
        ┌─────────────────┐                         ┌─────────────────┐
        │  Lid Control    │                         │ Fill Calculation│
        │ processAutoLid()│                         │ calculateBinLevel│
        └────────┬────────┘                         └────────┬────────┘
                 │                                           │
                 ▼                                           ▼
        ┌─────────────────┐                         ┌─────────────────┐
        │   SG90 Servo    │                         │ Alert + Display │
        │    Lid Control  │                         └────────┬────────┘
        └─────────────────┘                                  │
                                                             │
                         ┌───────────────────────────────────┼──────────────┐
                         │                                   │              │
                         ▼                                   ▼              ▼
                  ┌────────────┐                       ┌──────────┐   ┌──────────┐
                  │ 16×2 LCD   │                       │   LEDs   │   │  Buzzer  │
                  │ I2C A4/A5  │                       │ D12/D13  │   │   D10    │
                  └────────────┘                       └──────────┘   └──────────┘
```

---

## 🔄 System Workflow

```text
                    ┌──────────────────────┐
                    │   Arduino Uno Start │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ Read Hand Sensor     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ Read Bin-Level Sensor│
                    └──────────┬───────────┘
                               │
                ┌──────────────┴──────────────┐
                │                             │
                ▼                             ▼
       ┌──────────────────┐          ┌──────────────────┐
       │ Hand Detection   │          │ Fill Calculation │
       │ + Lid Control    │          │ + Range Clamp    │
       └────────┬─────────┘          └────────┬─────────┘
                │                             │
                ▼                             ▼
       ┌──────────────────┐          ┌──────────────────┐
       │ Servo / Lid      │          │ Alert Evaluation │
       └──────────────────┘          └────────┬─────────┘
                                              │
                         ┌────────────────────┼───────────────────┐
                         │                    │                   │
                         ▼                    ▼                   ▼
                      LCD                  LEDs                Buzzer
```

**Control flow per loop iteration:**

```text
Read hand sensor
      ↓
Read bin-level sensor
      ↓
Run automatic lid logic
      ↓
Calculate fill percentage
      ↓
Check full-bin threshold
      ↓
Every 300 ms:
Refresh LCD / LEDs / Buzzer / Serial telemetry
```

---

## 🔌 Circuit Connections

| Module                   | Pin             | Arduino Pin |
| ------------------------ | --------------- | ----------- |
| HC-SR04 Hand Sensor      | VCC / GND       | 5V / GND    |
| HC-SR04 Hand Sensor      | TRIG            | D7          |
| HC-SR04 Hand Sensor      | ECHO            | D6          |
| HC-SR04 Bin-Level Sensor | VCC / GND       | 5V / GND    |
| HC-SR04 Bin-Level Sensor | TRIG            | D5          |
| HC-SR04 Bin-Level Sensor | ECHO            | D4          |
| SG90 Servo               | V+ / GND        | 5V / GND    |
| SG90 Servo               | PWM Signal      | D9          |
| Buzzer                   | + / −           | D10 / GND   |
| Green LED                | Anode via 220 Ω | D12         |
| Red LED                  | Anode via 220 Ω | D13         |
| I2C LCD                  | SDA             | A4          |
| I2C LCD                  | SCL             | A5          |
| I2C LCD                  | VCC / GND       | 5V / GND    |

---

## 🧮 Bin-Level Calculation

The waste-level sensor converts the measured distance into a fill percentage using calibrated empty and full distances.

```text
distanceRange
= BIN_EMPTY_DISTANCE_CM − BIN_FULL_DISTANCE_CM
= 30 − 3
= 27 cm

filledDistance
= BIN_EMPTY_DISTANCE_CM − wasteDistanceCm

fillPercentage
= map(filledDistance, 0, distanceRange, 0, 100)

fillPercentage
= constrain(fillPercentage, 0, 100)
```

### Calibration Parameters

| Constant                 | Value | Meaning                                        |
| ------------------------ | ----: | ---------------------------------------------- |
| `BIN_EMPTY_DISTANCE_CM`  | 30 cm | Sensor-to-waste distance when the bin is empty |
| `BIN_FULL_DISTANCE_CM`   |  3 cm | Sensor-to-waste distance when the bin is full  |
| `FULL_THRESHOLD_PERCENT` |   85% | Fill level at which the alert is activated     |

The closer the measured distance is to **3 cm**, the higher the calculated fill percentage.

The closer the measured distance is to **30 cm**, the lower the calculated fill percentage.

Values outside the calibrated range are constrained between **0% and 100%** to prevent invalid results caused by sensor noise or abnormal readings.

---

## 🖐️ Automatic Lid Control

The first HC-SR04 sensor is dedicated to detecting a hand or object near the bin.

```text
Hand Distance ≤ 15 cm
        ↓
Hand Detected
        ↓
Servo → 90°
        ↓
Lid OPEN
```

When the hand is removed:

```text
Hand Removed
        ↓
3 Second Hold Period
        ↓
Servo → 0°
        ↓
Lid CLOSED
```

The lid timing is handled using `millis()` rather than `delay()`, allowing the rest of the system to continue operating simultaneously.

---

## 🚨 Alert System

The bin enters the alert state when the calculated fill level reaches the configured threshold.

```text
Fill Percentage ≥ 85%
        ↓
   BIN FULL
        ↓
┌───────┼────────┐
│       │        │
▼       ▼        ▼
Red    Buzzer   LCD
LED    Alert    Alert
Blink
```

### Alert Behaviour

| Condition | Green LED | Red LED  | Buzzer | Status         |
| --------- | --------- | -------- | ------ | -------------- |
| Normal    | ON        | OFF      | OFF    | NORMAL         |
| Full Bin  | OFF       | Blinking | Active | ALERT_BIN_FULL |

The red LED blinking and buzzer generation are also handled without blocking the main control loop.

---

## 🖥️ LCD Display

The 16×2 I2C LCD provides live local feedback.

It displays:

* Lid state
* Current fill percentage
* Normal/full status

Example system states:

```text
LID: CLOSED
FILL: 0%
```

```text
LID: OPEN
FILL: 75%
```

```text
LID: CLOSED
FILL: 100%
```

---

## 📡 Serial Telemetry

The Serial Monitor provides real-time diagnostic information including:

* Hand sensor distance
* Bin-level distance
* Calculated fill percentage
* Lid state
* System status
* Full-bin alert state

Serial communication operates at:

```text
115200 baud
```

This makes the system easier to debug and validate during simulation and hardware testing.

---

## ⚙️ Non-Blocking Firmware Design

One of the primary engineering decisions in this project is the use of **non-blocking timing**.

Instead of:

```cpp
delay(3000);
```

the firmware uses:

```cpp
millis()
```

to track elapsed time.

This allows the system to perform multiple tasks concurrently:

```text
┌────────────────────────────────────────────┐
│              Main Loop                     │
├────────────────────────────────────────────┤
│ Hand Detection                             │
│ Bin-Level Measurement                      │
│ Lid State Management                       │
│ Servo Control                              │
│ Fill Calculation                           │
│ LCD Update                                 │
│ LED Control                                │
│ Buzzer Control                             │
│ Serial Telemetry                           │
└────────────────────────────────────────────┘
              ↓
        No blocking delay
```

This architecture is particularly important in embedded systems where one blocking operation can prevent other subsystems from responding on time.

---

## 📁 Folder Structure

```text
04-Smart-Dustbin-Embedded-System/
│
├── Output/
│   ├── 01_normal.png
│   ├── 02_lid_open.png
│   ├── 03_partial_fill.png
│   └── 04_full_alert.png
│
├── diagram.json
├── libraries.txt
├── sketch.ino
└── README.md
```

---

## 🛠️ Technology Stack

**Hardware / Embedded:**

Arduino Uno · 2 × HC-SR04 Ultrasonic Sensors · SG90 Servo · 16×2 I2C LCD · Green LED · Red LED · Buzzer

**Software:**

C/C++ · Arduino Framework · Wokwi

**Communication / Interfaces:**

Ultrasonic Ranging · I2C · PWM · Digital GPIO · Serial Communication

**Libraries:**

LiquidCrystal I2C · Servo

---

## ▶️ How to Run the Project

### 1. Open the Live Simulation

**[▶️ Open Live Wokwi Simulation](https://wokwi.com/projects/472357743514021889)**

The project contains the firmware, circuit definition, and required configuration.

### 2. Start the Simulation

Click:

```text
▶ Start Simulation
```

The Arduino initializes the ultrasonic sensors, servo, LCD, LEDs, buzzer, and Serial communication.

### 3. Open the Serial Monitor

Set the Serial Monitor to:

```text
115200 baud
```

Observe live distance, fill percentage, lid state, and alert information.

### 4. Test the Hand Sensor

Set the hand-detection sensor distance below:

```text
15 cm
```

The servo should rotate and open the lid.

### 5. Test the Bin-Level Sensor

Reduce the bin-level sensor distance to simulate increasing waste.

The displayed fill percentage should increase accordingly.

---

## 🧪 Simulation Steps

1. Open the project: **[wokwi.com/projects/472357743514021889](https://wokwi.com/projects/472357743514021889)**
2. Click **▶️ Start Simulation**.
3. Set the **Hand HC-SR04** distance below **15 cm**.
4. Confirm that the lid opens and the servo rotates.
5. Remove the simulated hand and verify that the lid automatically closes after approximately **3 seconds**.
6. Lower the **Bin HC-SR04** distance to simulate increasing waste.
7. Observe the fill percentage on the LCD.
8. Set the bin-level distance to approximately **4.5 cm or lower** to simulate **≥85% fill**.
9. Confirm that the red LED blinks and the buzzer activates.
10. Monitor the Serial Monitor for live telemetry.

---

## 🧪 Testing and Validation

| Test Case            | Input Condition                | Expected Behaviour                            | Result |
| -------------------- | ------------------------------ | --------------------------------------------- | ------ |
| Idle / Empty Bin     | Hand ≥ 15 cm, Waste = 30 cm    | Lid closed, 0% fill, green LED ON             | ✅ Pass |
| Hand Detected        | Hand ≤ 15 cm                   | Lid opens, servo → 90°                        | ✅ Pass |
| Lid Auto-Close       | Hand removed, 3 s elapsed      | Lid closes, servo → 0°                        | ✅ Pass |
| Partial Fill         | Waste distance ≈ 9–10 cm       | Fill ≈ 74–77%, NORMAL status                  | ✅ Pass |
| Full Bin             | Waste distance ≈ 3 cm          | Fill = 100%, red LED + buzzer                 | ✅ Pass |
| Out-of-Range Echo    | No echo / timeout              | Reading safely handled, no invalid percentage | ✅ Pass |
| Concurrent Operation | Hand + full bin simultaneously | Lid and alert logic operate independently     | ✅ Pass |

**7 / 7 test cases passed**, including concurrency and fault-handling scenarios.

---

## 📸 Output

### Normal — Bin Empty

![Normal](Output/01_normal.png)

`0% fill · lid closed · green LED ON`

---

### Lid Open — Hand Detected

![Lid Open](Output/02_lid_open.png)

`Hand detected (9–10 cm) · servo rotates lid open`

---

### Partial Fill

![Partial Fill](Output/03_partial_fill.png)

`~74–77% fill · still within normal range`

---

### Full Alert

![Full Alert](Output/04_full_alert.png)

`100% fill · red LED blinking · buzzer active · STATUS:ALERT_BIN_FULL`

---

## ⚠️ Known Limitations

* **Single-point sensing** — the bin-level sensor measures down the centre of the bin, so unevenly distributed waste may cause the measured fill level to differ from the actual volume.
* **No weight awareness** — the system measures waste height rather than mass.
* **Ultrasonic blind zone** — the HC-SR04 has a minimum reliable sensing distance, so waste beyond the calibrated full point cannot be differentiated further.
* **No persistent storage** — fill history and alerts are available only during the active session.
* **Single-bin scope** — the current design is not networked for fleet-wide waste management.
* **Environmental sensitivity** — dust, moisture, and reflective surfaces can affect ultrasonic measurements in real-world environments.
* **Simulation-to-hardware gap** — physical deployment would require additional calibration and environmental testing.

---

## 🚀 Roadmap

| Area                     | Planned Enhancements                                          |
| ------------------------ | ------------------------------------------------------------- |
| ☁️ **IoT Layer**         | ESP32 + MQTT/HTTP for remote fill-level monitoring            |
| 📊 **Dashboard**         | Web/mobile dashboard for real-time waste monitoring           |
| ⚙️ **Calibration**       | Automatic sensor calibration during startup                   |
| 🔋 **Power**             | Battery + solar charging for outdoor deployment               |
| ⚖️ **Sensing**           | Add a weight sensor for more accurate waste estimation        |
| 🏷️ **Fleet Management** | RFID/GSM-based identification for municipal waste fleets      |
| 🎛️ **Signal Quality**   | Median/debounce filtering for improved ultrasonic reliability |
| 📈 **Analytics**         | Historical fill-level data and collection-route optimization  |

---

## 🎓 Learning Outcomes

Practical experience with:

* Interfacing multiple ultrasonic sensors with a single microcontroller
* Designing non-blocking firmware using `millis()`
* Building independent embedded control loops
* Converting physical sensor measurements into application-level values
* Applying calibration and range constraints
* Controlling servo motors through PWM
* Driving I2C LCD peripherals
* Managing LEDs and buzzer outputs concurrently
* Implementing automatic state transitions
* Debugging through Serial telemetry
* Building and validating embedded systems in Wokwi
* Designing firmware with separation between sensing, control, display, and alert logic

---

## 💼 Skills Demonstrated

Embedded C/C++ · Arduino programming · Embedded Systems · Ultrasonic Sensor Interfacing · Sensor Fusion · Non-Blocking Firmware · `millis()` Timing · State Machine Logic · Sensor Calibration · Signal Conditioning · Range Clamping · Servo/PWM Control · I2C Communication · LCD Programming · GPIO Control · Buzzer Control · Serial Debugging · Wokwi Simulation · Hardware Prototyping · Technical Documentation · Git/GitHub.

---

## 👤 Author

**Subham Bhattacherjee**

**Project:** Smart Dustbin — Touchless Waste Monitoring Embedded System

**GitHub:** [View Repository](https://github.com/Subhamrbj/Embedded-Systems-Projects/tree/main/04-Smart-Dustbin-Embedded-System)

**Live Simulation:** [Wokwi Simulation](https://wokwi.com/projects/472357743514021889)

---

## 📜 License

Licensed under the **MIT License** — free to use, modify, and distribute for personal, academic, or commercial purposes, with attribution appreciated.

---

## ⭐ Project Summary

* Built a **fully non-blocking multi-sensor embedded system** using Arduino Uno and `millis()`-based timing.
* Integrated **two independent HC-SR04 sensors** for touchless hand detection and continuous waste-level monitoring.
* Implemented **automatic servo-controlled lid operation** with timeout-based closing.
* Developed a calibrated **0–100% fill-level calculation** using physical distance measurements.
* Added **range clamping** to prevent invalid fill percentages caused by abnormal sensor readings.
* Implemented **visual and audible full-bin alerts** using LEDs and a buzzer.
* Integrated a **16×2 I2C LCD** for live local monitoring.
* Implemented **Serial telemetry** for real-time debugging and validation.
* Verified **7/7 test cases**, including simultaneous lid operation and full-bin alert conditions.
* Documented practical limitations such as single-point sensing and the absence of weight awareness.
* Structured for reproducibility with `sketch.ino`, `diagram.json`, `libraries.txt`, `Output/`, and a public Wokwi simulation.

> **Project Type:** Embedded Systems / IoT / Smart Waste Management  ·  **Platform:** Arduino UNO  ·  **Language:** Embedded C/C++  ·  **Simulation:** Wokwi
