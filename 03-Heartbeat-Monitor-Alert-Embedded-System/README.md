# ❤️ Heartbeat Monitor with Alert System — Arduino Embedded Signal Processing

![Platform](https://img.shields.io/badge/platform-Arduino%20Uno-00979D?logo=arduino\&logoColor=white)
![Simulation](https://img.shields.io/badge/simulated%20on-Wokwi-1DA1F2)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue?logo=c%2B%2B)
![Status](https://img.shields.io/badge/status-Completed-brightgreen)
![License](https://img.shields.io/badge/license-MIT-green)

<p align="center">

[▶️ **Run Live Wokwi Simulation**](https://wokwi.com/projects/472334564347172865)

</p>

> An Arduino UNO-based real-time signal-processing pipeline that acquires a synthetic pulse waveform, filters it, detects peaks, measures inter-beat intervals, computes BPM, classifies heart-rate state, and drives LCD/LED/buzzer feedback — demonstrating the acquire → filter → detect → decide → alert architecture used in embedded biomedical and wearable-health systems.

**📄 [sketch.ino](sketch.ino)  |  🖼️ [Output](#-output)**

---
## 📑 Table of Contents

1. [Project Overview](#-project-overview)
2. [Repository Structure](#-Repository-structure)
3. [Objectives](#-objectives)
4. [Why This Project Matters](#-why-this-project-matters)
5. [Industry Relevance](#-industry-relevance)
6. [Key Features](#-key-features)
7. [Components Used](#-components-used)
8. [Embedded Concepts Applied](#-embedded-concepts-applied)
9. [System Architecture](#️-system-architecture)
10. [Signal Processing Pipeline](#-signal-processing-pipeline)
11. [BPM Calculation](#️-bpm-calculation)
12. [BPM Classification](#-bpm-classification)
13. [Circuit Connections](#-circuit-connections)
14. [Firmware Parameters](#️-firmware-parameters)
15. [How to Run](#️-how-to-run-the-project)
16. [Simulation Outputs](#-simulation-outputs)
17. [Testing and Validation](#-testing-and-validation)
18. [Educational Disclaimer](#️-educational-disclaimer)
19. [Limitations](#️-limitations)
20. [Roadmap](#-roadmap)
21. [Skills Demonstrated](#-skills-demonstrated)
22. [Author](#-author)
23. [License](#-license)
---

## 📌 Project Overview

The **Heartbeat Monitor with Alert System** is an Arduino UNO embedded prototype, fully validated in **Wokwi**, that models the firmware pipeline behind a heart-rate monitoring device.

A potentiometer generates a controllable synthetic PPG-like pulse signal. The Arduino reads the signal through its analog input, applies a moving-average filter, detects pulse peaks using hysteresis, measures the **Inter-Beat Interval (IBI)**, calculates **BPM**, classifies the result as **LOW**, **NORMAL**, or **HIGH**, and drives a 16×2 I2C LCD, green/red LEDs, and buzzer accordingly.

The complete system — signal generation, acquisition, filtering, detection, timing, classification, and alert generation — operates in real time inside a single Arduino sketch.

---

## 📁 Repository Structure

```text
03-Heartbeat-Monitor-Alert-Embedded-System/
│
├── Output/
│   ├── high-alert.png
│   ├── low-alert.png
│   └── normal.png
│
├── README.md
├── diagram.json
├── libraries.txt
└── sketch.ino
```

| File / Folder   | Purpose                                 |
| --------------- | --------------------------------------- |
| `Output/`       | Simulation output screenshots           |
| `README.md`     | Project documentation                   |
| `diagram.json`  | Wokwi circuit configuration             |
| `libraries.txt` | Required Wokwi library configuration    |
| `sketch.ino`    | Main Arduino Embedded C/C++ source code |

---

## 🎯 Objectives

* Acquire a controllable pulse-like analog signal.
* Generate a repeatable synthetic PPG-like waveform for simulation.
* Reduce signal noise using moving-average filtering.
* Detect pulse peaks using hysteresis-based logic.
* Measure the Inter-Beat Interval between detected beats.
* Calculate BPM from measured timing intervals.
* Classify the measured heart-rate state as LOW, NORMAL, or HIGH.
* Display BPM and system status through a 16×2 I2C LCD.
* Provide visual feedback through green and red LEDs.
* Generate an audible alarm for LOW and HIGH states.
* Stream structured signal-processing telemetry through the Serial Monitor.
* Demonstrate a complete real-time embedded signal-processing pipeline.

---

## 💼 Why This Project Matters

This project goes beyond a basic sensor-reading demonstration. It implements a complete **embedded digital signal-processing (DSP) pipeline** on an Arduino UNO, covering analog signal acquisition, synthetic signal generation, noise filtering, hysteresis-based peak detection, Inter-Beat Interval measurement, BPM calculation, threshold classification, and multi-channel alert control.

It demonstrates practical embedded-system engineering concepts such as **real-time processing, timing analysis, signal conditioning, state classification, display interfacing, alarm generation, and simulation-based validation** — all running on a resource-constrained 8-bit microcontroller.

**At a glance:**

|                          |                                                                                            |
| ------------------------ | ------------------------------------------------------------------------------------------ |
| 🎯 **Role demonstrated** | Embedded Firmware Engineer / Biomedical & IoT Systems Developer                            |
| 🔧 **Core stack**        | Arduino UNO · C/C++ · I2C LCD · Analog Signal Processing                                   |
| 🧪 **Validation**        | 3 classified operating states, verified through live Serial telemetry and output evidence  |
| 📦 **Deliverables**      | Firmware, circuit definition, telemetry logs, simulation outputs, documented test evidence |

---

## 🏭 Industry Relevance

| Domain                          | Application Demonstrated                                                 |
| ------------------------------- | ------------------------------------------------------------------------ |
| **Wearable Health Technology**  | Real-time pulse acquisition and BPM computation                          |
| **Biomedical Embedded Systems** | Filtering, peak detection, and physiological-style signal interpretation |
| **IoT Healthcare**              | Local edge processing and alert generation                               |
| **Patient Monitoring**          | Threshold-based state classification and alarm logic                     |
| **Fitness & Consumer Devices**  | Acquire → process → display → alert workflow                             |

**Engineering capabilities demonstrated:** Arduino/embedded C/C++ · analog signal acquisition · digital filtering · peak detection · hysteresis · time-domain feature extraction · real-time classification · I2C communication · LED/buzzer control · Serial telemetry · simulation-based validation.

---

## ✨ Key Features

* Arduino UNO-based real-time signal-processing firmware
* Controllable synthetic PPG-like pulse generation through potentiometer
* Analog signal acquisition through `A0`
* 5-sample moving-average filtering
* Hysteresis-based peak detection
* Minimum IBI guard against unrealistically fast triggers
* Inter-Beat Interval measurement
* Formula-based BPM calculation
* BPM constrained to a 30–220 range for simulation stability
* Three-state classification: **LOW / NORMAL / HIGH**
* 16×2 I2C LCD for live BPM and status
* Green LED for normal operation
* Red LED for alarm conditions
* Different red LED blink rates for LOW and HIGH states
* Buzzer-driven audible alarm
* Structured Serial telemetry
* Fully reproducible Wokwi simulation
* No physical hardware required for demonstration

---

## 🧩 Components Used

| Component     | Qty | Purpose                          |
| ------------- | --: | -------------------------------- |
| Arduino UNO   |   1 | Main microcontroller             |
| Potentiometer |   1 | Synthetic pulse/PPG signal input |
| 16×2 I2C LCD  |   1 | BPM and status display           |
| Green LED     |   1 | Normal status indication         |
| Red LED       |   1 | Alarm indication                 |
| 220Ω Resistor |   2 | LED current limiting             |
| Buzzer        |   1 | Audible alarm                    |

**Software / Tools:** Wokwi · Arduino IDE · `LiquidCrystal I2C` library

---

## 🧠 Embedded Concepts Applied

`Microcontroller Programming` · `Analog Signal Acquisition (ADC)` · `Synthetic Signal Generation` · `Moving-Average Filtering` · `Hysteresis-Based Peak Detection` · `Time-Domain Feature Extraction` · `Inter-Beat Interval Measurement` · `BPM Calculation` · `Threshold Classification` · `I2C Communication` · `Digital Output Control` · `Serial Telemetry` · `millis()` Timing · `Simulation-Based Testing`

---

## 🏗️ System Architecture

```text
                      ┌─────────────────────────┐
                      │      POTENTIOMETER      │
                      │  Synthetic Pulse Input  │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │      Arduino UNO        │
                      │       Analog A0         │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │ Synthetic Pulse Signal  │
                      │      Acquisition        │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │   Moving Average Filter │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │     Peak Detection      │
                      │       + Hysteresis      │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │    IBI Measurement      │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │     BPM Calculation     │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │    State Evaluation     │
                      └────────────┬────────────┘
                                   │
                           ┌───────┼───────┐
                           │       │       │
                           ▼       ▼       ▼
                        NORMAL    LOW     HIGH
                           │      ALARM   ALARM
                           │       │       │
                           ▼       ▼       ▼
                       Green LED  Red LED  Red LED
                                 + Buzzer + Buzzer
                           │       │       │
                           └───────┼───────┘
                                   ▼
                      ┌─────────────────────────┐
                      │       16×2 I2C LCD       │
                      │       BPM + Status       │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │      Serial Monitor     │
                      │      Real-Time Data     │
                      └─────────────────────────┘
```

---

## 🔬 Signal Processing Pipeline

### 1. Analog Signal Acquisition

The synthetic pulse signal is sampled through Arduino analog input `A0`.

```cpp
#define SENSOR_PIN A0
analogRead(SENSOR_PIN);
```

### 2. Synthetic Pulse Generation

The potentiometer position controls the simulated pulse period:

```text
1500 ms → ~40 BPM
1000 ms → ~60 BPM
800 ms  → ~75 BPM
600 ms  → ~100 BPM
428 ms  → ~140 BPM
```

This allows the simulation to sweep across LOW, NORMAL, and HIGH operating states.

### 3. Moving-Average Filtering

```cpp
const int FILTER_SIZE = 5;
```

Five samples are buffered and averaged before peak detection to smooth the incoming signal.

```text
Raw Signal
    ↓
5 Samples
    ↓
Average
    ↓
Filtered Signal
```

### 4. Hysteresis-Based Peak Detection

```cpp
const int SENSOR_THRESHOLD = 550;
const int RESET_THRESHOLD  = 500;
```

The signal must rise above the beat threshold to trigger detection and then fall below the reset threshold before another beat can be detected.

```text
Signal rises above 550
        ↓
    Beat Detected
        ↓
Signal must fall below 500
        ↓
Detection Re-Armed
```

This prevents repeated detection of the same pulse.

### 5. Inter-Beat Interval Measurement

```text
IBI = Current Beat Time − Previous Beat Time
```

A minimum interval is used to reject unrealistically fast triggers:

```cpp
const int MIN_IBI = 300;
```

---

## ❤️ BPM Calculation

The BPM is calculated directly from the measured Inter-Beat Interval:

```text
BPM = 60,000 / IBI
```

### Example

```text
IBI = 800 ms

BPM = 60,000 / 800

BPM = 75
```

The corresponding firmware calculation is:

```cpp
int rawBPM = 60000UL / ibi;
```

The resulting value is constrained to **30–220 BPM** for simulation stability.

---

## 🚦 BPM Classification

The classification thresholds are defined as:

```cpp
const int LOW_BPM_THRESHOLD  = 60;
const int HIGH_BPM_THRESHOLD = 100;
```

| BPM Range     | State      | Green LED | Red LED       | Buzzer |
| ------------- | ---------- | --------- | ------------- | ------ |
| Below 60 BPM  | LOW ALARM  | OFF       | Blinking      | ON     |
| 60–100 BPM    | NORMAL     | ON        | OFF           | OFF    |
| Above 100 BPM | HIGH ALARM | OFF       | Fast Blinking | ON     |

The LOW and HIGH states activate the alarm outputs, while the NORMAL state indicates stable operation.

---

## 🔌 Circuit Connections

| Component     | Pin     | Arduino UNO |
| ------------- | ------- | ----------- |
| Potentiometer | GND     | GND         |
| Potentiometer | VCC     | 5V          |
| Potentiometer | SIG     | A0          |
| LCD           | VCC     | 5V          |
| LCD           | GND     | GND         |
| LCD           | SDA     | A4          |
| LCD           | SCL     | A5          |
| Buzzer        | Signal  | D11         |
| Buzzer        | GND     | GND         |
| Green LED     | Anode   | D12         |
| Green LED     | Cathode | 220Ω → GND  |
| Red LED       | Anode   | D13         |
| Red LED       | Cathode | 220Ω → GND  |

---

## ⚙️ Firmware Parameters

| Parameter        |       Value |
| ---------------- | ----------: |
| Controller       | Arduino UNO |
| Analog Input     |          A0 |
| Filter Size      |   5 samples |
| Sensor Threshold |         550 |
| Reset Threshold  |         500 |
| Minimum IBI      |      300 ms |
| Minimum BPM      |          30 |
| Maximum BPM      |         220 |
| LOW Threshold    |    < 60 BPM |
| NORMAL Range     |  60–100 BPM |
| HIGH Threshold   |   > 100 BPM |
| LCD              |    16×2 I2C |
| Buzzer Pin       |         D11 |
| Green LED        |         D12 |
| Red LED          |         D13 |

---

## ▶️ How to Run the Project

### 1. Open the Live Simulation

[▶️ **Open Live Wokwi Simulation**](https://wokwi.com/projects/472334564347172865)

The project contains the Arduino firmware, circuit configuration, required library configuration, and simulation environment.

### 2. Start the Simulation

Click **▶ Start Simulation**.

The LCD initializes and then displays the live BPM and current classification state.

### 3. Control the Simulated Heart Rate

Rotate the **potentiometer** to change the synthetic pulse period and move between:

```text
LOW ALARM
NORMAL
HIGH ALARM
```

### 4. Observe the Output

| Condition  | Green LED | Red LED       | Buzzer |
| ---------- | --------- | ------------- | ------ |
| Normal     | ON        | OFF           | OFF    |
| Low Alarm  | OFF       | Blinking      | ON     |
| High Alarm | OFF       | Fast Blinking | ON     |

### 5. Monitor Serial Telemetry

The Serial Monitor exposes:

```text
RAW_ADC
FILTERED
IBI_MS
BPM
STATE
```

This provides visibility into the complete signal-processing pipeline.

---

## 📸 Simulation Outputs

### Normal Heart Rate

![Normal Heart Rate](Output/normal.png)

```text
Green LED → ON
Red LED   → OFF
Buzzer    → OFF
LCD       → STATUS: NORMAL
```

### Low Heart Rate Alarm

![Low Heart Rate Alarm](Output/low-alert.png)

```text
Green LED → OFF
Red LED   → Blinking
Buzzer    → ON
LCD       → STATUS: LOW
```

### High Heart Rate Alarm

![High Heart Rate Alarm](Output/high-alert.png)

```text
Green LED → OFF
Red LED   → Fast Blinking
Buzzer    → ON
LCD       → STATUS: HIGH
```

---

## 🧪 Testing and Validation

### Test Case 1 — Normal

```text
LCD Status: NORMAL
Green LED: ON
Red LED: OFF
Buzzer: OFF

IBI_MS:725 | BPM:82 | STATE:NORMAL
IBI_MS:853 | BPM:70 | STATE:NORMAL
```

### Test Case 2 — Low Alarm

```text
LCD Status: LOW
Green LED: OFF
Red LED: Blinking
Buzzer: ON

IBI_MS:1024 | BPM:58 | STATE:ALARM_LOW
IBI_MS:1049 | BPM:57 | STATE:ALARM_LOW
IBI_MS:1203 | BPM:49 | STATE:ALARM_LOW
```

### Test Case 3 — High Alarm

```text
LCD Status: HIGH
Green LED: OFF
Red LED: Fast Blinking
Buzzer: ON

IBI_MS:556 | BPM:107 | STATE:ALARM_HIGH
IBI_MS:501 | BPM:119 | STATE:ALARM_HIGH
IBI_MS:500 | BPM:120 | STATE:ALARM_HIGH
```

### Validation Summary

| Test Case  | BPM Condition | LCD    | Green LED | Red LED       | Buzzer |
| ---------- | ------------- | ------ | --------- | ------------- | ------ |
| Normal     | 60–100 BPM    | NORMAL | ON        | OFF           | OFF    |
| Low Alarm  | <60 BPM       | LOW    | OFF       | Blinking      | ON     |
| High Alarm | >100 BPM      | HIGH   | OFF       | Fast Blinking | ON     |

**3 / 3 operating states verified** through live Serial telemetry and documented simulation output evidence.

---

## ⚠️ Educational Disclaimer

> **This is strictly an educational embedded-systems prototype and simulation. It is not a medical device and must not be used for medical diagnosis, treatment, emergency monitoring, or clinical decision-making.**

The signal source is a **potentiometer-generated synthetic PPG-like waveform** in Wokwi rather than a real pulse sensor.

The BPM thresholds used in this project are demonstration values for simulation and should not be interpreted as medical recommendations.

A real healthcare device would require validated sensors, calibration, advanced signal processing, clinical evaluation, safety testing, and applicable regulatory approval.

---

## ⚠️ Limitations

* **Synthetic pulse input** — the project uses a potentiometer-generated PPG-like waveform instead of a physical pulse sensor.
* **Simulation-based validation** — the system has been validated in Wokwi rather than on a clinical measurement system.
* **Basic signal processing** — moving-average filtering and threshold-based peak detection are used instead of advanced biomedical signal-processing algorithms.
* **Fixed thresholds** — `<60`, `60–100`, and `>100` BPM are demonstration values and are not medical recommendations.
* **No motion-artifact handling.**
* **No historical data storage.**
* **No wireless connectivity** in the Arduino UNO implementation.
* **No mobile application.**
* **No medical validation, clinical testing, or regulatory certification.**

---

## 🚀 Roadmap

| Area                     | Planned Enhancements                                                                                         |
| ------------------------ | ------------------------------------------------------------------------------------------------------------ |
| 🔧 **Hardware**          | Real pulse/PPG sensor such as MAX30102, OLED upgrade, ESP32 migration, battery support, wearable form factor |
| 🔬 **Signal Processing** | Adaptive filtering, improved peak detection, motion-artifact rejection, HRV analysis, signal-quality scoring |
| 🌐 **Connectivity**      | ESP32 Wi-Fi/Bluetooth, mobile application, web dashboard, cloud monitoring, remote alerts                    |
| 💾 **Data**              | SD-card/database logging, real-time graphing, long-term trend tracking                                       |
| ⚙️ **System**            | Configurable thresholds, refined alarm logic, battery monitoring, low-power operation                        |

---

## 💼 Skills Demonstrated

Embedded C/C++ · Arduino UNO programming · analog signal acquisition · ADC · synthetic signal generation · moving-average filtering · hysteresis-based peak detection · Inter-Beat Interval measurement · BPM calculation · threshold classification · I2C communication · LCD interfacing · GPIO control · LED indication · buzzer control · `millis()`-based real-time timing · Serial telemetry · embedded debugging · Wokwi simulation · Git/GitHub · technical documentation.

---

## 👤 Author

**Subham Bhattacherjee**

**Project:** Heartbeat Monitor with Alert System

**Program:** M.Tech Computer Science & Engineering — Kalyani University

**Areas of Interest:** Embedded Systems · Artificial Intelligence · Machine Learning · Research and Development

**GitHub:** [View Repository](https://github.com/Subhamrbj/Embedded-Systems-Projects/tree/main/03-Heartbeat-Monitor-Alert-Embedded-System)

**Live Simulation:** [Run on Wokwi](https://wokwi.com/projects/472334564347172865)

---

## 📜 License

Licensed under the **MIT License** — free to use, modify, and distribute for personal, academic or commercial purposes, with attribution appreciated.

---

## ⭐ Project Summary

* Built a complete **embedded signal-processing pipeline** on Arduino UNO: acquisition → filtering → peak detection → feature extraction → classification → alerting.
* Implemented **moving-average filtering** and **hysteresis-based peak detection** to improve pulse-event detection.
* Calculated **BPM from measured Inter-Beat Interval**, using direct signal-processing mathematics rather than lookup tables.
* Designed a **three-state classification system** covering LOW, NORMAL, and HIGH operating conditions.
* Implemented distinct **LED and buzzer responses** for normal and alarm states.
* Interfaced a **16×2 I2C LCD** for live BPM and status visualization.
* Exposed the complete processing pipeline through structured **Serial telemetry** for debugging and validation.
* Validated **3 / 3 operating states** with documented test evidence and simulation screenshots.
* Structured for reproducibility with `sketch.ino`, `diagram.json`, `libraries.txt`, `Output/`, and a public Wokwi simulation link.

> **Project Type:** Embedded Systems / Biomedical Signal Processing / IoT Healthcare  ·  **Platform:** Arduino UNO  ·  **Language:** Embedded C/C++  ·  **Simulation:** Wokwi
