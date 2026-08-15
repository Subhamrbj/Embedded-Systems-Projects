# ❤️ Heartbeat Monitor with Alert System

## 📌 Overview

The **Heartbeat Monitor with Alert System** is an educational embedded-systems prototype based on **Arduino UNO** and **Wokwi simulation**. The system monitors a simulated heartbeat signal, processes the signal, calculates **Beats Per Minute (BPM)** using **Inter-Beat Interval (IBI)** measurement, displays the heart-rate information on a **16×2 I2C LCD**, and generates visual and audible alerts when the calculated BPM crosses predefined thresholds.

A potentiometer is used to generate a controllable synthetic PPG-like pulse signal. The Arduino UNO reads the analog input, generates the simulated pulse waveform, applies a moving-average filter, detects pulse peaks, measures the time between beats, calculates BPM, classifies the heart-rate condition, and controls the display and alert outputs.

The project is implemented using **Embedded C/C++** and is designed primarily for educational and academic demonstration.

---

## 🎯 Problem Statement

Heart-rate monitoring systems need to acquire pulse signals, process the signal, determine the interval between successive beats, calculate BPM, identify abnormal conditions, and provide an appropriate alert.

This project demonstrates these fundamental operations using an **Arduino UNO** and a simulated pulse signal.

The system focuses on:

* Analog signal acquisition
* Synthetic pulse generation
* Signal filtering
* Peak detection
* Inter-Beat Interval measurement
* BPM calculation
* Threshold-based classification
* LCD interfacing
* Serial communication
* LED-based status indication
* Audible alarm generation
* Real-time embedded processing

---

## ⚠️ Educational Disclaimer

> **This project is strictly an educational embedded-systems prototype and simulation. It is not a medical device and must not be used for medical diagnosis, treatment, emergency monitoring, or clinical decision-making.**

The current implementation uses a **potentiometer-generated synthetic PPG-like signal** in Wokwi rather than a real human pulse sensor.

The BPM thresholds used in this project are demonstration thresholds for the embedded-system simulation and should not be interpreted as medical recommendations.

A real healthcare device would require appropriate sensors, calibration, signal processing, validation, safety testing, clinical evaluation, and regulatory approval.

---

## 🏭 Industry Relevance

The embedded concepts demonstrated by this project are relevant to several real-world application areas, including:

* Wearable health-monitoring systems
* Fitness monitoring devices
* Pulse monitoring systems
* Patient monitoring prototypes
* Biomedical embedded systems
* IoT healthcare systems
* Real-time sensor-processing systems
* Edge computing applications
* Alert and notification systems
* Smart wearable devices

The project demonstrates the basic embedded architecture behind systems that acquire sensor data, process it locally, make decisions, and provide real-time feedback.

---

# ✨ Features

* Arduino UNO-based implementation
* Synthetic PPG-like pulse generation
* Potentiometer-controlled simulated heart rate
* Analog input acquisition
* Moving-average filtering
* Pulse peak detection
* Inter-Beat Interval measurement
* BPM calculation
* BPM range classification
* Normal heart-rate indication
* Low-BPM alarm
* High-BPM alarm
* 16×2 I2C LCD display
* Serial Monitor telemetry
* Green LED status indication
* Red LED alarm indication
* Buzzer-based audible alarm
* Different alert behavior for low and high BPM
* Wokwi simulation
* Modular Embedded C/C++ implementation

---

# 🧩 Components Used

| Component     | Quantity | Purpose                          |
| ------------- | -------: | -------------------------------- |
| Arduino UNO   |        1 | Main microcontroller             |
| Potentiometer |        1 | Synthetic pulse/PPG signal input |
| 16×2 I2C LCD  |        1 | BPM and status display           |
| Green LED     |        1 | Normal status indication         |
| Red LED       |        1 | Alarm indication                 |
| 220Ω Resistor |        2 | LED current limiting             |
| Buzzer        |        1 | Audible alarm                    |
| Wokwi         |        1 | Circuit simulation platform      |

---

# 🧠 Embedded Concepts Used

### 1. Microcontroller Programming

The Arduino UNO acts as the central processing unit of the system.

It reads the input signal, processes the data, calculates BPM, evaluates the system state, and controls the output devices.

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

The potentiometer controls the simulated pulse period.

The implementation maps the potentiometer position approximately as follows:

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

# ❤️ BPM Calculation

The BPM is calculated from the **Inter-Beat Interval (IBI)**.

## Formula

```text
BPM = 60,000 / IBI
```

Where:

* **BPM** = Beats Per Minute
* **IBI** = Inter-Beat Interval in milliseconds
* **60,000** = milliseconds in one minute

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

The calculated value is constrained to:

```text
30–220 BPM
```

for the simulation.

---

# 🏗️ Architecture

```text
                   ┌─────────────────────────┐
                   │      POTENTIOMETER      │
                   │ Synthetic Pulse Input   │
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
                    ┌────────────┼────────────┐
                    │            │            │
                    ▼            ▼            ▼
                 NORMAL         LOW          HIGH
                               ALARM         ALARM
                    │            │            │
                    ▼            ▼            ▼
                Green LED     Red LED       Red LED
                               + Buzzer      + Buzzer

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

# 🔌 Circuit Connections

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

# 🚦 BPM Classification

The system uses the following thresholds:

```cpp
const int LOW_BPM_THRESHOLD  = 60;
const int HIGH_BPM_THRESHOLD = 100;
```

The classification is:

```text
              BPM
               │
         ┌─────┼─────┐
         │     │     │
         ▼     ▼     ▼
       < 60  60–100  >100
         │     │     │
         ▼     ▼     ▼
        LOW  NORMAL  HIGH
       ALARM          ALARM
```

| BPM Range     | State      | Green LED | Red LED       | Buzzer |
| ------------- | ---------- | --------- | ------------- | ------ |
| 60–100 BPM    | NORMAL     | ON        | OFF           | OFF    |
| Below 60 BPM  | LOW ALARM  | OFF       | Blinking      | ON     |
| Above 100 BPM | HIGH ALARM | OFF       | Fast Blinking | ON     |

---

# 📁 Folder Structure

```text
03-Heartbeat-Monitor-Alert-Embedded-System/
│
├── sketch/
│   └── sketch.ino
│
├── wokwi/
│   ├── diagram.json
│   └── libraries.txt
│
├── screenshots/
│   ├── normal.png
│   ├── low-alert.png
│   └── high-alert.png
│
└── README.md
```

---

# 🛠️ Installation

## Requirements

The project can be demonstrated using:

* Wokwi
* Arduino UNO simulation
* GitHub
* Web browser

No physical hardware is required for the current simulation.

The project uses the **LiquidCrystal I2C** library, with the library configuration provided through:

```text
wokwi/libraries.txt
```

---

# ▶️ Simulation Steps

### Step 1 — Open the Wokwi Project

Open the Wokwi project:

```text
https://wokwi.com/projects/472334564347172865
```

### Step 2 — Add the Source Code

Use:

```text
sketch/sketch.ino
```

### Step 3 — Configure the Circuit

Use:

```text
wokwi/diagram.json
```

### Step 4 — Add the Library

The project uses:

```text
LiquidCrystal I2C
```

The library configuration is provided through:

```text
wokwi/libraries.txt
```

### Step 5 — Start Simulation

Click:

```text
Start Simulation
```

### Step 6 — Observe the LCD

The startup screen displays:

```text
HEART MONITOR
INITIALIZING...
```

After initialization, the LCD displays the BPM and system status.

### Step 7 — Adjust the Potentiometer

Rotate the potentiometer to change the simulated pulse period.

This allows the system to demonstrate:

```text
NORMAL
LOW ALARM
HIGH ALARM
```

conditions.

### Step 8 — Observe LEDs and Buzzer

The outputs change according to the calculated BPM.

### Step 9 — Open Serial Monitor

Observe:

```text
RAW_ADC
FILTERED
IBI_MS
BPM
STATE
```

---

# ▶️ How to Run

The project is primarily designed for **Wokwi simulation**.

### Wokwi

1. Open the Wokwi project.
2. Load `sketch.ino`.
3. Load the circuit configuration from `diagram.json`.
4. Ensure `LiquidCrystal I2C` is included.
5. Start the simulation.
6. Adjust the potentiometer.
7. Observe BPM on the LCD.
8. Observe the LEDs.
9. Observe the buzzer during alarm conditions.
10. Open the Serial Monitor.

---

# 📸 Screenshots

The repository should contain screenshots demonstrating the three major operating conditions.

## 1. Normal Heart Rate

File:

```text
screenshots/normal.png
```

Expected behavior:

```text
Green LED → ON
Red LED   → OFF
Buzzer    → OFF
LCD       → STATUS: NORMAL
```

## 2. Low Heart Rate Alarm

File:

```text
screenshots/low-alert.png
```

Expected behavior:

```text
Green LED → OFF
Red LED   → Blinking
Buzzer    → ON
LCD       → STATUS: LOW
```

## 3. High Heart Rate Alarm

File:

```text
screenshots/high-alert.png
```

Expected behavior:

```text
Green LED → OFF
Red LED   → Fast Blinking
Buzzer    → ON
LCD       → STATUS: HIGH
```

---

# 🧪 Test Results

The system was tested in the Wokwi simulation under three operating conditions.

## Test Case 1 — Normal

### Condition

```text
60–100 BPM
```

### Observed

```text
LCD Status: NORMAL
Green LED: ON
Red LED: OFF
Buzzer: OFF
```

Example:

```text
IBI_MS:725 | BPM:82 | STATE:NORMAL
IBI_MS:853 | BPM:70 | STATE:NORMAL
```

## Test Case 2 — Low Alarm

### Condition

```text
BPM < 60
```

### Observed

```text
LCD Status: LOW
Green LED: OFF
Red LED: Blinking
Buzzer: ON
```

Example:

```text
IBI_MS:1024 | BPM:58 | STATE:ALARM_LOW
IBI_MS:1049 | BPM:57 | STATE:ALARM_LOW
IBI_MS:1203 | BPM:49 | STATE:ALARM_LOW
```

## Test Case 3 — High Alarm

### Condition

```text
BPM > 100
```

### Observed

```text
LCD Status: HIGH
Green LED: OFF
Red LED: Fast Blinking
Buzzer: ON
```

Example:

```text
IBI_MS:556 | BPM:107 | STATE:ALARM_HIGH
IBI_MS:501 | BPM:119 | STATE:ALARM_HIGH
IBI_MS:500 | BPM:120 | STATE:ALARM_HIGH
```

### Test Summary

| Test Case  | BPM Condition | LCD    | Green LED | Red LED       | Buzzer |
| ---------- | ------------- | ------ | --------- | ------------- | ------ |
| Normal     | 60–100 BPM    | NORMAL | ON        | OFF           | OFF    |
| Low Alarm  | <60 BPM       | LOW    | OFF       | Blinking      | ON     |
| High Alarm | >100 BPM      | HIGH   | OFF       | Fast Blinking | ON     |

---

# ⚠️ Limitations

1. **Synthetic Pulse Input**
   The current project does not use a physical pulse sensor. A potentiometer is used to generate a controllable synthetic PPG-like signal in Wokwi.

2. **Simulation-Based Implementation**
   The current demonstration is performed using Wokwi rather than a physical medical sensor and patient measurement system.

3. **Basic Signal Processing**
   The project uses a basic moving-average filter and threshold-based peak detection. Real biomedical signals can require considerably more advanced processing.

4. **Fixed Thresholds**
   The current implementation uses:

   ```text
   Low Alarm → <60 BPM
   Normal    → 60–100 BPM
   High Alarm → >100 BPM
   ```

   These are demonstration thresholds and are not medical recommendations.

5. **Motion Artifacts**
   The system does not currently implement advanced motion-artifact detection or rejection.

6. **No Historical Data Storage**
   The current version does not store historical BPM measurements.

7. **No Wireless Communication**
   The current Arduino UNO implementation does not provide Wi-Fi, Bluetooth, or cloud connectivity.

8. **No Mobile Application**
   The project currently provides LCD and Serial Monitor outputs rather than a dedicated mobile application.

9. **No Medical Validation**
   The project has not undergone clinical testing, medical validation, or medical-device certification.

---

# 🚀 Future Improvements

### Hardware Improvements

* Integrate a real pulse sensor
* Integrate a PPG sensor
* Integrate MAX30102
* Add an OLED display
* Upgrade to ESP32
* Add rechargeable battery support
* Develop a wearable hardware prototype

### Signal Processing Improvements

* Advanced digital filtering
* Adaptive thresholding
* Improved peak detection
* Motion-artifact rejection
* Noise reduction
* Heart-rate variability analysis
* More robust signal-quality assessment

### Connectivity Improvements

* ESP32 Wi-Fi connectivity
* Bluetooth communication
* Mobile application
* Cloud-based monitoring
* Web dashboard
* Remote alert notifications

### Data Management

* Historical BPM logging
* SD card storage
* Database integration
* Real-time graphs
* Long-term monitoring

### System Improvements

* User-specific configuration
* Configurable thresholds
* Improved alarm logic
* Battery monitoring
* Low-power operation
* Real-time data visualization

---

# 📚 Learning Outcomes

This project provides practical experience in:

* Embedded C/C++ programming
* Arduino UNO programming
* Analog signal acquisition
* Sensor simulation
* Synthetic signal generation
* Moving-average filtering
* Peak detection
* Inter-Beat Interval measurement
* BPM calculation
* Threshold-based classification
* State-based embedded logic
* LCD interfacing
* I2C communication
* LED control
* Buzzer control
* Serial communication
* Real-time processing
* `millis()`-based timing
* Wokwi circuit simulation
* Embedded-system debugging
* GitHub project organization
* Technical documentation

---

# 👨‍💻 Author

## Subham Bhattacherjee

**M.Tech Computer Science & Engineering**
**Kalyani University**

### Areas of Interest

* Embedded Systems
* Artificial Intelligence
* Machine Learning
* Data Science
* Computer Science
* Research and Development
