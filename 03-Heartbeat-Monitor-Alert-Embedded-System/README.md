# ❤️ Heartbeat Monitor with Alert System

![Platform](https://img.shields.io/badge/platform-Arduino%20Uno-00979D?logo=arduino&logoColor=white)
![Simulation](https://img.shields.io/badge/simulated%20on-Wokwi-1DA1F2)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue?logo=c%2B%2B)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-Educational%20Prototype-yellow)

## 📑 Table of Contents

1. [Overview](#-overview)
2. [Problem Statement](#-problem-statement)
3. [Educational Disclaimer](#️-educational-disclaimer)
4. [Industry Relevance](#-industry-relevance)
5. [Features](#-features)
6. [Components Used](#-components-used)
7. [Embedded Concepts Used](#-embedded-concepts-used)
8. [BPM Calculation](#-bpm-calculation)
9. [Architecture](#️-architecture)
10. [Circuit Connections](#-circuit-connections)
11. [BPM Classification](#-bpm-classification)
12. [Folder Structure](#-folder-structure)
13. [Installation](#️-installation)
14. [Simulation & How to Run](#️-simulation--how-to-run)
15. [Simulation Outputs](#-simulation-outputs)
16. [Test Results](#-test-results)
17. [Limitations](#️-limitations)
18. [Future Improvements](#-future-improvements)
19. [Learning Outcomes](#-learning-outcomes)
20. [License](#-license)
21. [Author](#-author)

---

## 📌 Overview

The **Heartbeat Monitor with Alert System** is an educational embedded-systems prototype based on **Arduino UNO** and **Wokwi simulation**. The system monitors a simulated heartbeat signal, processes the signal, calculates **Beats Per Minute (BPM)** using **Inter-Beat Interval (IBI)** measurement, displays the heart-rate information on a **16×2 I2C LCD**, and generates visual and audible alerts when the calculated BPM crosses predefined thresholds.

A potentiometer is used to generate a controllable synthetic PPG-like pulse signal. The Arduino UNO reads the analog input, generates the simulated pulse waveform, applies a moving-average filter, detects pulse peaks, measures the time between beats, calculates BPM, classifies the heart-rate condition, and controls the display and alert outputs.

The project is implemented using **Embedded C/C++** and is designed primarily for educational and academic demonstration.

---

## 🎯 Problem Statement

Heart-rate monitoring systems need to acquire pulse signals, process the signal, determine the interval between successive beats, calculate BPM, identify abnormal conditions, and provide an appropriate alert.

This project demonstrates these fundamental operations using an **Arduino UNO** and a simulated pulse signal.

The system focuses on:

- Analog signal acquisition
- Synthetic pulse generation
- Signal filtering
- Peak detection
- Inter-Beat Interval measurement
- BPM calculation
- Threshold-based classification
- LCD interfacing
- Serial communication
- LED-based status indication
- Audible alarm generation
- Real-time embedded processing

---

## ⚠️ Educational Disclaimer

> **This project is strictly an educational embedded-systems prototype and simulation. It is not a medical device and must not be used for medical diagnosis, treatment, emergency monitoring, or clinical decision-making.**

The current implementation uses a **potentiometer-generated synthetic PPG-like signal** in Wokwi rather than a real human pulse sensor.

The BPM thresholds used in this project are demonstration thresholds for the embedded-system simulation and should not be interpreted as medical recommendations.

A real healthcare device would require appropriate sensors, calibration, signal processing, validation, safety testing, clinical evaluation, and regulatory approval.

---

## 🏭 Industry Relevance

The embedded concepts demonstrated by this project are relevant to several real-world application areas, including:

- Wearable health-monitoring systems
- Fitness monitoring devices
- Pulse monitoring systems
- Patient monitoring prototypes
- Biomedical embedded systems
- IoT healthcare systems
- Real-time sensor-processing systems
- Edge computing applications
- Alert and notification systems
- Smart wearable devices

The project demonstrates the basic embedded architecture behind systems that acquire sensor data, process it locally, make decisions, and provide real-time feedback.

---

## ✨ Features

- Arduino UNO-based implementation
- Synthetic PPG-like pulse generation
- Potentiometer-controlled simulated heart rate
- Analog input acquisition
- Moving-average filtering
- Pulse peak detection
- Inter-Beat Interval measurement
- BPM calculation
- BPM range classification
- Normal heart-rate indication
- Low-BPM alarm
- High-BPM alarm
- 16×2 I2C LCD display
- Serial Monitor telemetry
- Green LED status indication
- Red LED alarm indication
- Buzzer-based audible alarm
- Different alert behavior for low and high BPM
- Wokwi simulation
- Modular Embedded C/C++ implementation

---

## 🧩 Components Used

| Component | Quantity | Purpose |
|---|---:|---|
| Arduino UNO | 1 | Main microcontroller |
| Potentiometer | 1 | Synthetic pulse/PPG signal input |
| 16×2 I2C LCD | 1 | BPM and status display |
| Green LED | 1 | Normal status indication |
| Red LED | 1 | Alarm indication |
| 220Ω Resistor | 2 | LED current limiting |
| Buzzer | 1 | Audible alarm |

**Software/Tools:** [Wokwi](https://wokwi.com) (circuit simulation), Arduino IDE (firmware development), `LiquidCrystal I2C` library.

---

## 🧠 Embedded Concepts Used

### 1. Microcontroller Programming

The Arduino UNO acts as the central processing unit of the system. It reads the input signal, processes the data, calculates BPM, evaluates the system state, and controls the output devices.

### 2. Analog Signal Acquisition

The synthetic pulse signal is connected to Arduino analog pin `A0`.

```cpp
#define SENSOR_PIN A0
```

The signal is read using:

```cpp
analogRead(SENSOR_PIN);
```

### 3. Synthetic Pulse Generation

The potentiometer controls the simulated pulse period. The implementation maps the potentiometer position approximately as follows:

```text
1500 ms → approximately 40 BPM
1000 ms → approximately 60 BPM
800 ms  → approximately 75 BPM
600 ms  → approximately 100 BPM
428 ms  → approximately 140 BPM
```

This allows different heart-rate conditions to be demonstrated without requiring physical sensor hardware.

### 4. Moving-Average Filtering

The project uses a moving-average filter to smooth the generated signal.

```cpp
const int FILTER_SIZE = 5;
```

Five signal samples are maintained and averaged before peak detection.

### 5. Peak Detection

A heartbeat is detected when the filtered signal crosses the sensor threshold.

```cpp
const int SENSOR_THRESHOLD = 550;
```

A reset threshold is also used:

```cpp
const int RESET_THRESHOLD = 500;
```

The signal must fall below the reset threshold before another peak can be detected. This provides basic hysteresis for the peak-detection process.

### 6. Inter-Beat Interval Measurement

The system measures the time between consecutive detected pulse peaks.

```text
IBI = Current Beat Time - Previous Beat Time
```

The minimum accepted IBI is:

```cpp
const int MIN_IBI = 300;
```

This prevents unrealistically short intervals from being accepted by the system.

---

## ❤️ BPM Calculation

The BPM is calculated from the **Inter-Beat Interval (IBI)**.

### Formula

```text
BPM = 60,000 / IBI
```

Where:
- **BPM** = Beats Per Minute
- **IBI** = Inter-Beat Interval in milliseconds
- **60,000** = milliseconds in one minute

### Example

If:

```text
IBI = 800 ms
```

then:

```text
BPM = 60,000 / 800
    = 75 BPM
```

The implementation calculates BPM using:

```cpp
int rawBPM = 60000UL / ibi;
```

The calculated value is constrained to **30–220 BPM** for the simulation.

---

## 🏗️ Architecture

```text
                    ┌─────────────────────────┐
                    │      POTENTIOMETER      │
                    │  Synthetic Pulse Input  │
                    └────────────┬────────────┘
                                 │
                                 ▼
                          ┌─────────────────┐
                          │ Arduino UNO A0  │
                          │  Analog Input   │
                          └────────┬────────┘
                                   │
                                   ▼
                      ┌────────────────────────┐
                      │ Synthetic Pulse        │
                      │ Generation             │
                      └───────────┬────────────┘
                                  │
                                  ▼
                      ┌────────────────────────┐
                      │ Moving Average Filter  │
                      └───────────┬────────────┘
                                  │
                                  ▼
                      ┌────────────────────────┐
                      │ Peak Detection         │
                      └───────────┬────────────┘
                                  │
                                  ▼
                      ┌────────────────────────┐
                      │ IBI Measurement        │
                      └───────────┬────────────┘
                                  │
                                  ▼
                      ┌────────────────────────┐
                      │ BPM Calculation        │
                      └───────────┬────────────┘
                                  │
                                  ▼
                      ┌────────────────────────┐
                      │ State Evaluation       │
                      └───────────┬────────────┘
                                  │
                       ┌──────────┼──────────┐
                       │          │          │
                       ▼          ▼          ▼
                    NORMAL       LOW        HIGH
                                ALARM      ALARM
                       │          │          │
                       ▼          ▼          ▼
                   Green LED   Red LED    Red LED
                                + Buzzer   + Buzzer
                                  │
                                  ▼
                         ┌──────────────────┐
                         │   16×2 I2C LCD   │
                         │   BPM + Status   │
                         └──────────────────┘
                                  │
                                  ▼
                         ┌──────────────────┐
                         │  Serial Monitor  │
                         │  Real-Time Data  │
                         └──────────────────┘
```

---

## 🔌 Circuit Connections

| Component | Pin | Arduino UNO |
|---|---|---|
| Potentiometer | GND | GND |
| Potentiometer | VCC | 5V |
| Potentiometer | SIG | A0 |
| LCD | VCC | 5V |
| LCD | GND | GND |
| LCD | SDA | A4 |
| LCD | SCL | A5 |
| Buzzer | Signal | D11 |
| Buzzer | GND | GND |
| Green LED | Anode | D12 |
| Green LED | Cathode | 220Ω → GND |
| Red LED | Anode | D13 |
| Red LED | Cathode | 220Ω → GND |

---

## 🚦 BPM Classification

The system uses the following thresholds:

```cpp
const int LOW_BPM_THRESHOLD  = 60;
const int HIGH_BPM_THRESHOLD = 100;
```

| BPM Range | State | Green LED | Red LED | Buzzer |
|---|---|---|---|---|
| Below 60 BPM | LOW ALARM | OFF | Blinking | ON |
| 60–100 BPM | NORMAL | ON | OFF | OFF |
| Above 100 BPM | HIGH ALARM | OFF | Fast Blinking | ON |

---

## 📁 Folder Structure

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

| File / Folder | Purpose |
|---|---|
| `sketch.ino` | Main Arduino Embedded C/C++ source code |
| `diagram.json` | Wokwi circuit configuration |
| `libraries.txt` | Required Wokwi library configuration |
| `Output/` | Simulation output screenshots |
| `README.md` | Project documentation |

---

## 🛠️ Installation

The project can be demonstrated using Wokwi, Arduino UNO simulation, GitHub, and a web browser — no physical hardware is required for the current simulation.

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) if you plan to flash real hardware (optional — not required for Wokwi simulation).
2. Install the required library via **Library Manager**: `LiquidCrystal I2C` (also declared in `libraries.txt`).
3. Wire the circuit as described in [Circuit Connections](#-circuit-connections), or open `diagram.json` directly in [Wokwi](https://wokwi.com).
4. Open `sketch.ino` in the Arduino IDE, select **Arduino Uno** as the board, and upload (for real hardware) — or open the project directly in Wokwi to simulate.

---

## ▶️ Simulation & How to Run

1. Open the Wokwi project: **https://wokwi.com/projects/472334564347172865**
2. Click **Start Simulation**. The LCD shows a startup screen (`HEART MONITOR` / `INITIALIZING...`), then switches to live BPM and status.
3. Rotate the **potentiometer** to change the simulated pulse period — this drives the system between `NORMAL`, `LOW ALARM`, and `HIGH ALARM` conditions.
4. Observe the LED and buzzer response for each condition:

   | Condition | Green LED | Red LED | Buzzer |
   |---|---|---|---|
   | Normal | ON | OFF | OFF |
   | Low Alarm | OFF | Blinking | ON |
   | High Alarm | OFF | Fast Blinking | ON |

5. Open the **Serial Monitor** to view live telemetry (`RAW_ADC`, `FILTERED`, `IBI_MS`, `BPM`, `STATE`).

To run on real hardware instead: wire the circuit per the [Circuit Connections](#-circuit-connections) table, upload `sketch.ino` via the Arduino IDE, and open the Serial Monitor at the configured baud rate to view the same telemetry.

---

## 📸 Simulation Outputs

### 1. Normal Heart Rate

![Normal Heart Rate](Output/normal.png)

```text
Green LED → ON
Red LED   → OFF
Buzzer    → OFF
LCD       → STATUS: NORMAL
```

### 2. Low Heart Rate Alarm

![Low Heart Rate Alarm](Output/low-alert.png)

```text
Green LED → OFF
Red LED   → Blinking
Buzzer    → ON
LCD       → STATUS: LOW
```

### 3. High Heart Rate Alarm

![High Heart Rate Alarm](Output/high-alert.png)

```text
Green LED → OFF
Red LED   → Fast Blinking
Buzzer    → ON
LCD       → STATUS: HIGH
```

---

## 🧪 Test Results

The system was tested in the Wokwi simulation under three operating conditions.

### Test Case 1 — Normal (60–100 BPM)

```text
LCD Status: NORMAL | Green LED: ON | Red LED: OFF | Buzzer: OFF
```

Example telemetry:
```text
IBI_MS:725 | BPM:82 | STATE:NORMAL
IBI_MS:853 | BPM:70 | STATE:NORMAL
```

### Test Case 2 — Low Alarm (BPM < 60)

```text
LCD Status: LOW | Green LED: OFF | Red LED: Blinking | Buzzer: ON
```

Example telemetry:
```text
IBI_MS:1024 | BPM:58 | STATE:ALARM_LOW
IBI_MS:1049 | BPM:57 | STATE:ALARM_LOW
IBI_MS:1203 | BPM:49 | STATE:ALARM_LOW
```

### Test Case 3 — High Alarm (BPM > 100)

```text
LCD Status: HIGH | Green LED: OFF | Red LED: Fast Blinking | Buzzer: ON
```

Example telemetry:
```text
IBI_MS:556 | BPM:107 | STATE:ALARM_HIGH
IBI_MS:501 | BPM:119 | STATE:ALARM_HIGH
IBI_MS:500 | BPM:120 | STATE:ALARM_HIGH
```

### Test Summary

| Test Case | BPM Condition | LCD | Green LED | Red LED | Buzzer |
|---|---|---|---|---|---|
| Normal | 60–100 BPM | NORMAL | ON | OFF | OFF |
| Low Alarm | <60 BPM | LOW | OFF | Blinking | ON |
| High Alarm | >100 BPM | HIGH | OFF | Fast Blinking | ON |

*(Results captured from Wokwi simulation runs; see `Output/` for corresponding screenshots.)*

---

## ⚠️ Limitations

1. **Synthetic pulse input** — the project does not use a physical pulse sensor. A potentiometer generates a controllable synthetic PPG-like signal in Wokwi.
2. **Simulation-based implementation** — the demonstration runs in Wokwi rather than on a physical medical sensor and patient measurement system.
3. **Basic signal processing** — a simple moving-average filter and threshold-based peak detection are used; real biomedical signals typically need considerably more advanced processing.
4. **Fixed thresholds** — `<60 BPM` (low), `60–100 BPM` (normal), `>100 BPM` (high) are demonstration thresholds, not medical recommendations.
5. **No motion-artifact handling** — the system does not implement motion-artifact detection or rejection.
6. **No historical data storage** — BPM measurements are not persisted beyond the current session.
7. **No wireless communication** — the Arduino UNO implementation has no Wi-Fi, Bluetooth, or cloud connectivity.
8. **No mobile application** — output is limited to the LCD and Serial Monitor.
9. **No medical validation** — the project has not undergone clinical testing, medical validation, or medical-device certification.

---

## 🚀 Future Improvements

**Hardware**
- Integrate a real pulse/PPG sensor (e.g. MAX30102)
- Add an OLED display
- Upgrade to ESP32
- Add rechargeable battery support
- Develop a wearable hardware prototype

**Signal Processing**
- Advanced digital filtering and adaptive thresholding
- Improved peak detection
- Motion-artifact rejection and noise reduction
- Heart-rate variability analysis
- Signal-quality assessment

**Connectivity**
- ESP32 Wi-Fi / Bluetooth communication
- Mobile application and web dashboard
- Cloud-based monitoring and remote alert notifications

**Data Management**
- Historical BPM logging (SD card or database)
- Real-time graphing
- Long-term monitoring

**System**
- User-specific configuration and configurable thresholds
- Improved alarm logic
- Battery monitoring and low-power operation

---

## 📚 Learning Outcomes

This project provides practical experience in:

- Embedded C/C++ and Arduino UNO programming
- Analog signal acquisition and synthetic signal generation
- Moving-average filtering and peak detection
- Inter-Beat Interval measurement and BPM calculation
- Threshold-based classification and state-based embedded logic
- LCD interfacing and I2C communication
- LED and buzzer control
- Serial communication and `millis()`-based real-time timing
- Wokwi circuit simulation and embedded-system debugging
- GitHub project organization and technical documentation

---

## 📄 License

This project is licensed under the **MIT License** — free to use, modify, and distribute for personal, academic, or commercial purposes, with attribution appreciated.

---

## 👨‍💻 Author

**Subham Bhattacherjee**
M.Tech Computer Science & Engineering — Kalyani University

**Areas of Interest:** Embedded Systems · Artificial Intelligence · Machine Learning · Data Science · Research and Development

Simulation: https://wokwi.com/projects/472334564347172865
