# 🗑️ Smart Dustbin — Touchless Waste Monitoring Embedded System

![Platform](https://img.shields.io/badge/platform-Arduino%20Uno-00979D?logo=arduino&logoColor=white)
![Simulation](https://img.shields.io/badge/simulated%20on-Wokwi-1DA1F2)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue?logo=c%2B%2B)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-Prototype-yellow)

> An Arduino-based touchless waste bin controller that opens its lid automatically on hand detection, continuously tracks fill level with a second ultrasonic sensor, and escalates into a visual + audible alert past a configurable threshold — all running on a fully non-blocking control loop with zero `delay()` calls in the main path.

**🔗 [Live Wokwi Simulation](https://wokwi.com/projects/472357743514021889) &nbsp;|&nbsp; 📄 [sketch.ino](#) &nbsp;|&nbsp; 🖼️ [Screenshots](#-screenshots)**

---

## 💼 Why This Project Matters

The standout engineering decision in this project is architectural: the entire control loop is **non-blocking**, built entirely on `millis()`-based timing instead of `delay()`. That single choice is what lets two independent ultrasonic sensors, a servo, an LCD, dual LEDs, and a buzzer all update concurrently without ever stalling each other — exactly the discipline required in real embedded products, where a blocking call in one subsystem can freeze the whole device. On top of that, the fill-level calculation includes explicit clamping to reject out-of-range sensor noise, and the README documents real physical limitations (single-point sensing, no weight awareness) rather than glossing over them.

**At a glance:**

| | |
|---|---|
| 🎯 **Role demonstrated** | Embedded Firmware Engineer — sensor fusion & non-blocking control |
| 🔧 **Core stack** | Arduino UNO · C/C++ · Dual Ultrasonic Sensing · I2C · PWM |
| 🧪 **Validation** | 7 test cases including concurrency and fault handling — all passed |
| 📦 **Deliverables** | Firmware, circuit definition, telemetry, documented test evidence |

---

## 📑 Table of Contents

1. [Overview](#-overview)
2. [Problem Statement](#-problem-statement)
3. [Industry Relevance](#-industry-relevance)
4. [Features](#-features)
5. [Components Used](#-components-used)
6. [Embedded Concepts Applied](#-embedded-concepts-applied)
7. [System Architecture](#️-system-architecture)
8. [Circuit Connections](#-circuit-connections)
9. [Folder Structure](#-folder-structure)
10. [Installation](#️-installation)
11. [Simulation Steps (Wokwi)](#️-simulation-steps-wokwi)
12. [How to Run](#-how-to-run)
13. [Bin-Level Formula](#-bin-level-formula)
14. [Screenshots](#-screenshots)
15. [Test Results](#-test-results)
16. [Known Limitations](#️-known-limitations)
17. [Roadmap](#-roadmap)
18. [Learning Outcomes](#-learning-outcomes)
19. [Author](#-author)
20. [License](#-license)

---

## 📖 Overview

Public and industrial dustbins are usually opened by hand — spreading germs — and overflow unnoticed until someone physically checks them. This project solves both problems with a single low-cost Arduino Uno controller:

* A **hand-detection ultrasonic sensor** opens the lid automatically when an object/hand comes within range, and auto-closes it after a hold period.
* A **second ultrasonic sensor** continuously measures fill level and converts distance into a 0–100% fill percentage.
* An **LCD** shows lid state and fill percentage live.
* **LEDs + a buzzer** escalate into a full-bin alert once fill level crosses a configurable threshold — with non-blocking blinking and tone generation.

---

## 🎯 Problem Statement

Manually operated waste bins have three recurring problems:

1. Require physical contact with the lid, spreading contamination.
2. Give no indication of fill status until they visibly overflow.
3. Rely on scheduled (rather than need-based) collection, wasting manpower and fuel.

This project addresses all three with contactless operation and real-time fill monitoring — laying the groundwork for a networked "smart city" waste-collection system.

---

## 🏭 Industry Relevance

| Domain | Application in this project |
|---|---|
| **Smart City / Municipal Waste Management** | Need-based collection routing instead of fixed schedules |
| **Touchless Public Infrastructure** | Hygienic, contactless interaction points (elevators, dispensers, bins) |
| **Industrial IoT (IIoT) Monitoring** | Same sensor-fusion + threshold-alerting pattern used in tank-level and silo monitoring |
| **Embedded Systems Engineering** | Non-blocking state machines, sensor calibration, and filtered readings — core firmware skills |

---

## ✨ Features

* 🖐️ Touchless, automatic lid opening via ultrasonic hand detection
* ⏱️ Auto-close after a configurable hold time — no lid left open indefinitely
* 📊 Real-time bin fill percentage (0–100%) from a dedicated second ultrasonic sensor
* 🚨 Full-bin alert at a configurable threshold (default 85%)
* 🟢🔴 Status LEDs — steady green for normal, blinking red for full
* 🔊 Buzzer tone alert when the bin is full
* 🖥️ 16×2 I2C LCD live status display (lid state + fill %)
* 📡 Serial telemetry for every reading — distance, fill %, lid state, system status
* ⚙️ **Fully non-blocking `loop()`** — zero `delay()` calls in the main control path, so every sensor and alert updates smoothly and simultaneously

---

## 🔧 Components Used

| Component | Qty | Purpose |
|---|---:|---|
| Arduino Uno | 1 | Main microcontroller |
| HC-SR04 Ultrasonic Sensor | 2 | Hand detection + bin level sensing |
| SG90 Servo Motor | 1 | Lid actuation |
| 16×2 I2C LCD (PCF8574, addr `0x27`) | 1 | Status display |
| Buzzer | 1 | Audible full-bin alert |
| Green LED | 1 | Normal status indicator |
| Red LED | 1 | Full-bin alert indicator |
| 220 Ω Resistor | 2 | LED current limiting |
| Jumper wires / breadboard | — | Wiring |

---

## 🧠 Embedded Concepts Applied

`Ultrasonic Ranging (pulseIn)` · `Sensor Fusion — Two Independent Control Loops` · `Non-Blocking Timing (millis())` · `State Machines` · `Signal Conditioning & Clamping` · `Analog-to-Percentage Mapping (map())` · `I2C Communication` · `PWM Actuation` · `Serial Telemetry`

---

## 🏗️ System Architecture

```text
                ┌───────────────────────────┐
                │        Arduino Uno         │
                │                           │
 HC-SR04 #1 ───►│  Hand Detection (D6/D7)   │
 (Hand)         │            │              │
                │            ▼              │
                │   processAutoLid()        │──► Servo (D9) — Lid
                │            │              │
 HC-SR04 #2 ───►│  Bin Level (D4/D5)        │
 (Waste Level)  │            │              │
                │            ▼              │
                │  calculateBinLevel()      │
                │            │              │
                │            ▼              │
                │ updateAlertsAndDisplay()  │──► LCD (I2C: A4/A5)
                │            │              │──► Green LED (D12)
                │            │              │──► Red LED (D13)
                │            │              │──► Buzzer (D10)
                │            ▼              │
                │      Serial Monitor        │
                └───────────────────────────┘
```

**Control flow per loop iteration:**

```text
Read hand sensor → Read bin-level sensor → Run lid logic
   → Recalculate fill % → Every 300 ms: refresh LCD / LEDs / buzzer / Serial log
```

---

## 🔌 Circuit Connections

| Module | Pin | Arduino Pin |
|---|---|---|
| HC-SR04 (Hand) | VCC / GND | 5V / GND |
| HC-SR04 (Hand) | TRIG | D7 |
| HC-SR04 (Hand) | ECHO | D6 |
| HC-SR04 (Bin Level) | VCC / GND | 5V / GND |
| HC-SR04 (Bin Level) | TRIG | D5 |
| HC-SR04 (Bin Level) | ECHO | D4 |
| Servo | V+ / GND | 5V / GND |
| Servo | PWM (signal) | D9 |
| Buzzer | + / − | D10 / GND |
| Green LED | Anode (via 220 Ω) | D12 |
| Red LED | Anode (via 220 Ω) | D13 |
| I2C LCD | SDA | A4 |
| I2C LCD | SCL | A5 |
| I2C LCD | VCC / GND | 5V / GND |

---

## 📁 Folder Structure

```text
04-Smart-Dustbin-Embedded-System/
├── Output/
│   ├── 01_normal.png        # Empty bin, lid closed
│   ├── 02_lid_open.png      # Hand detected, lid open
│   ├── 03_partial_fill.png  # Bin at 74–77% fill
│   └── 04_full_alert.png    # Bin at 100%, full alert active
├── diagram.json              # Wokwi circuit/wiring definition
├── libraries.txt              # Required library list
├── sketch.ino                 # Main firmware source
└── README.md
```

---

## ⚙️ Installation

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) (or use Wokwi directly — no install needed).
2. Install the required libraries via **Library Manager**: `LiquidCrystal I2C` and `Servo` (bundled with Arduino IDE).
3. Wire the circuit per [Circuit Connections](#-circuit-connections), or open `diagram.json` directly in [Wokwi](https://wokwi.com).
4. Open `sketch.ino`, select **Arduino Uno**, and upload.

---

## ▶️ Simulation Steps (Wokwi)

1. Open the project: **[wokwi.com/projects/472357743514021889](https://wokwi.com/projects/472357743514021889)**
2. Click ▶️ **Start Simulation**.
3. Set the **Hand HC-SR04** distance slider below 15 cm to simulate a hand near the bin — the lid should open and the servo should rotate.
4. Lower the **Bin HC-SR04** distance slider (closer to 3 cm) to simulate waste filling up — watch the LCD fill percentage rise.
5. Push the bin-level distance to ≤ ~4.5 cm (≥ 85% fill) to trigger the full-bin alert — red LED blinks, buzzer sounds.
6. Watch the **Serial Monitor** for live telemetry of every reading.

---

## 🚀 How to Run

**On real hardware:**
1. Wire up the components per the circuit table.
2. Upload `sketch.ino` via the Arduino IDE.
3. Open the Serial Monitor at **115200 baud** for live telemetry.
4. Wave a hand near the hand-detection sensor to open the lid; it auto-closes after 3 seconds.
5. Watch the LCD and LEDs as fill level changes.

**In simulation:** follow the [Simulation Steps](#️-simulation-steps-wokwi) above.

---

## 📐 Bin-Level Formula

```text
distanceRange   = BIN_EMPTY_DISTANCE_CM − BIN_FULL_DISTANCE_CM     (30 − 3 = 27 cm)
filledDistance  = BIN_EMPTY_DISTANCE_CM − wasteDistanceCm
fillPercentage  = map(filledDistance, 0, distanceRange, 0, 100)
fillPercentage  = constrain(fillPercentage, 0, 100)
```

The closer the measured echo distance is to `BIN_FULL_DISTANCE_CM` (3 cm), the higher the fill %; the closer to `BIN_EMPTY_DISTANCE_CM` (30 cm), the lower. Readings outside this physical range are clamped **before** the calculation runs, so noise or sensor glitches can never produce an invalid percentage.

| Constant | Value | Meaning |
|---|---|---|
| `BIN_EMPTY_DISTANCE_CM` | 30 cm | Sensor-to-waste distance when bin is empty |
| `BIN_FULL_DISTANCE_CM` | 3 cm | Sensor-to-waste distance when bin is full |
| `FULL_THRESHOLD_PERCENT` | 85% | Fill % at which the full-bin alert triggers |

---

## 📸 Screenshots

### Normal — Bin Empty
![Normal](Output/01_normal.png)
`0% fill · lid closed · green LED on`

### Lid Open — Hand Detected
![Lid Open](Output/02_lid_open.png)
`Hand detected (9–10 cm) · servo rotates lid open`

### Partial Fill
![Partial Fill](Output/03_partial_fill.png)
`~74–77% fill · still within normal range`

### Full Alert
![Full Alert](Output/04_full_alert.png)
`100% fill · red LED blinking · buzzer active · STATUS:ALERT_BIN_FULL on Serial`

---

## ✅ Test Results

| Test Case | Input Condition | Expected Behaviour | Result |
|---|---|---|---|
| Idle / empty bin | Hand ≥ 15 cm, Waste = 30 cm | Lid closed, 0% fill, green LED on | ✅ Pass |
| Hand detected | Hand ≤ 15 cm | Lid opens (servo → 90°), stays open while hand present | ✅ Pass |
| Lid auto-close | Hand removed, 3 s elapsed | Lid closes (servo → 0°) automatically | ✅ Pass |
| Partial fill | Waste distance ≈ 9–10 cm | Fill % ≈ 74–77%, still `STATUS:NORMAL` | ✅ Pass |
| Full bin | Waste distance ≈ 3 cm | Fill = 100%, red LED blinks, buzzer tone, `STATUS:ALERT_BIN_FULL` | ✅ Pass |
| Out-of-range echo | No echo / timeout | Reading clamped/defaulted, no crash or invalid % | ✅ Pass |
| Concurrent operation | Hand + full bin simultaneously | Lid logic and alert logic run independently without blocking each other | ✅ Pass |

**7 / 7 test cases passed** — including fault-tolerance and true-concurrency scenarios, not just the straightforward path.

---

## ⚠️ Known Limitations

* **Single-point sensing** — the bin-level sensor reads down the bin's center, so unevenly piled or soft/absorbent waste (paper, fabric) can be under- or over-estimated versus true volume.
* **No weight awareness** — the system measures fill *height*, not mass; light crumpled paper reads the same as dense compacted waste.
* **Ultrasonic blind zone** — HC-SR04 sensors have a minimum reliable sensing distance (~2 cm); waste piled higher than the `BIN_FULL_DISTANCE_CM` calibration point isn't distinguished further.
* **No persistent storage** — fill history and alerts exist only on the Serial Monitor for the current session.
* **Single-bin scope** — the current design isn't yet networked for fleet-wide municipal monitoring.
* **Environmental sensitivity** — ultrasonic readings can be affected by dust, moisture, or reflective surfaces in real-world bins, more so than in simulation.

---

## 🚀 Roadmap

| Area | Planned Enhancements |
|---|---|
| ☁️ **IoT Layer** | ESP32 + MQTT/HTTP to push fill-level data to a dashboard for route optimization |
| ⚙️ **Calibration** | Auto-calibration routine run at first boot, replacing fixed constants |
| 🔋 **Power** | Battery + solar charging for outdoor deployment, low-power sleep between readings |
| ⚖️ **Sensing** | Add a weight sensor alongside ultrasonic ranging for more accurate fill estimation |
| 🏷️ **Fleet Management** | RFID/GSM-based bin identification for municipal fleet tracking |
| 🎛️ **Signal Quality** | Debounce/median filtering on ultrasonic readings to further reduce noise |

---

## 🎓 Learning Outcomes

Practical experience interfacing **multiple ultrasonic sensors** on one microcontroller · designing **non-blocking, multi-tasking firmware** using `millis()` instead of `delay()` · sensor calibration and mapping physical measurements to application-level values · driving I2C peripherals (LCD) alongside PWM (servo) and digital I/O (LEDs, buzzer) concurrently · building and testing embedded systems entirely in simulation before physical deployment · structuring firmware with clear separation of concerns (sensing, control logic, alerting/display).

---

## 👤 Author

**Subham Bhattacherjee**
**Project:** Smart Dustbin — Touchless Waste Monitoring Embedded System
**GitHub:** [View Repository](https://github.com/Subhamrbj/Embedded-Systems-Projects/tree/main/04-Smart-Dustbin-Embedded-System)
**Live Simulation:** [wokwi.com/projects/472357743514021889](https://wokwi.com/projects/472357743514021889)

---

## 📜 License

Licensed under the **MIT License** — free to use, modify, and distribute for personal, academic, or commercial purposes, with attribution appreciated.

---

## ⭐ Project Summary

**Highlights for recruiters:**

* Built a **fully non-blocking, multi-sensor control loop** — zero `delay()` calls in the main path — coordinating two ultrasonic sensors, a servo, an LCD, dual LEDs, and a buzzer simultaneously.
* Implemented **sensor fusion** across two independent HC-SR04 units driving two separate control loops (hand detection + fill monitoring).
* Applied **signal conditioning and clamping** so out-of-range or noisy echo readings can never produce an invalid fill percentage.
* Designed **timeout-based state transitions** for the auto-closing lid, avoiding both stuck-open and premature-close failure modes.
* Delivered a real 0–100% fill-mapping formula from calibrated physical distances, not a placeholder.
* Verified **7 test cases including concurrency and fault-injection scenarios** — not just the happy path.
* Documented honest, specific **known limitations** (single-point sensing, no weight awareness) — the mark of an engineer who understands the deployment gap, not just the demo.

> **Project Type:** Embedded Systems / IoT / Smart Waste Management &nbsp;·&nbsp; **Platform:** Arduino UNO &nbsp;·&nbsp; **Language:** Embedded C/C++ &nbsp;·&nbsp; **Simulation:** Wokwi
