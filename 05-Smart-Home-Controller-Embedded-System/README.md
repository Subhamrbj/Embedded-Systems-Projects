# 🏠 Smart Home Controller – Industry-Oriented Embedded System

![Platform](https://img.shields.io/badge/platform-ESP32-E7352C?logo=espressif\&logoColor=white)
![Simulation](https://img.shields.io/badge/simulated%20on-Wokwi-1DA1F2)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue?logo=c%2B%2B)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-Prototype-yellow)

An **ESP32-based smart-home automation and security controller** that integrates environmental sensing, automatic lighting, temperature-based fan control, manual appliance override, motion detection, security monitoring, OLED visualization, and Serial telemetry into a single embedded system.

The controller continuously monitors **temperature, humidity, ambient light, and motion**, processes these inputs using embedded control logic, and drives simulated appliances and security indicators accordingly.

The complete prototype was designed and functionally validated using the **Wokwi virtual simulation environment**.

---

## 📑 Table of Contents

1. [Overview](#-overview)
2. [Problem Statement](#-problem-statement)
3. [Objectives](#-objectives)
4. [Industry Relevance](#-industry-relevance)
5. [Features](#-features)
6. [Components Used](#-components-used)
7. [Technologies Used](#-technologies-used)
8. [Embedded Concepts Used](#-embedded-concepts-used)
9. [Architecture](#-architecture)
10. [Circuit Connections](#-circuit-connections)
11. [Automation Logic](#-automation-logic)
12. [Manual Override](#-manual-override)
13. [Security System](#-security-system)
14. [Folder Structure](#-folder-structure)
15. [Installation](#-installation)
16. [How to Run](#-how-to-run)
17. [Wokwi Simulation](#-wokwi-simulation)
18. [Test Scenarios](#-test-scenarios)
19. [Simulation Output](#-simulation-output)
20. [Screenshots](#-screenshots)
21. [Test Results](#-test-results)
22. [Known Limitations](#-known-limitations)
23. [Future Improvements](#-future-improvements)
24. [Learning Outcomes](#-learning-outcomes)
25. [Author](#-author)
26. [License](#-license)

---

## 📖 Overview

Traditional home-control systems often depend on manual operation of appliances and provide limited environmental awareness. This can result in unnecessary energy usage, reduced convenience, and limited integration between automation and security functions.

This project addresses these challenges through a centralized **ESP32-based smart-home controller** capable of combining:

* 🌡️ Temperature monitoring
* 💧 Humidity monitoring
* 💡 Automatic room-light control
* 🌬️ Automatic fan control
* 🕹️ Manual appliance control
* 🌙 Ambient-light detection
* 🚶 Motion detection
* 🔐 Security monitoring
* 🚨 Alarm indication
* 🖥️ OLED status visualization
* 📡 Serial telemetry

The ESP32 continuously reads sensor inputs, evaluates control conditions, updates the actuators, manages security states, and reports the current system condition through the OLED display and Serial Monitor.

### System Monitoring

The controller monitors:

* Temperature
* Humidity
* Ambient light
* Human motion
* Room-light state
* Fan state
* Light operating mode
* Fan operating mode
* Security state
* Alarm state

---

## 🎯 Problem Statement

Conventional room-control systems may have several limitations:

1. Appliances often require manual operation.
2. Lights may remain ON unnecessarily.
3. Fans may operate without considering room temperature.
4. Environmental conditions are not continuously monitored.
5. Motion-based automation may be unavailable.
6. Security monitoring is often separate from appliance control.
7. Manual override is not always available when automation is active.

This project addresses these limitations by implementing a centralized embedded controller capable of **sensor-based automation, manual control, environmental monitoring, and security detection** using an ESP32 microcontroller.

---

## 🎯 Objectives

The primary objectives of the project are:

* Develop an ESP32-based smart-home controller.
* Monitor temperature and humidity using a DHT22 sensor.
* Measure ambient brightness using an LDR.
* Detect human motion using a PIR sensor.
* Automatically control room lighting according to environmental conditions and motion.
* Automatically control a fan using temperature thresholds.
* Implement temperature hysteresis for stable fan operation.
* Provide manual override for room-light control.
* Provide manual override for fan control.
* Implement an independent security subsystem.
* Detect motion while the security system is armed.
* Provide visual and audible security alerts.
* Display real-time system information.
* Provide Serial Monitor telemetry for debugging and verification.
* Validate multiple operating scenarios using Wokwi simulation.

---

## 🏭 Industry Relevance

This prototype demonstrates concepts directly applicable to:

* **Smart Home Automation** – centralized control of lighting and appliances.
* **Smart Building Systems** – sensor-driven environmental control.
* **Energy Management** – automatic appliance operation based on actual conditions.
* **IoT and Edge Devices** – local processing of sensor information.
* **Environmental Monitoring** – continuous temperature, humidity, and light sensing.
* **Security Systems** – motion-based intrusion detection.
* **Embedded Firmware Development** – real-time sensor processing and actuator control.
* **Industrial Automation** – threshold-based control and state management.

### Engineering Skills Demonstrated

* ESP32 firmware development
* Embedded C/C++ programming
* GPIO programming
* ADC-based sensing
* I2C communication
* Sensor interfacing
* Threshold-based automation
* Hysteresis-based control
* Manual override handling
* Event detection
* State management
* Actuator control
* Security-event handling
* Serial telemetry
* Simulation-based testing

> **Hardware note:** For physical deployment, real appliances must be controlled using appropriately rated and electrically isolated relay or solid-state switching hardware.

---

## ✨ Features

### 🌡️ Environmental Monitoring

* Temperature measurement using DHT22
* Humidity measurement using DHT22
* Ambient-light measurement using LDR
* Human-motion detection using PIR
* Real-time sensor status
* OLED visualization
* Serial telemetry

### 💡 Automatic Lighting

The lighting subsystem combines **ambient-light information and motion detection** to determine whether the room light should be activated.

```text
                 Read LDR
                    ↓
              Is Room Dark?
                /       \
              YES        NO
               │          │
               ▼          ▼
           Read PIR     Light OFF
               │
            Motion?
            /    \
          YES     NO
           │       │
           ▼       ▼
       Light ON  Check Timeout
                    │
                    ▼
                Light OFF
```

### Lighting Behaviour

| Condition              | Result               |
| ---------------------- | -------------------- |
| Dark + Motion Detected | Light ON             |
| Dark + No Motion       | Timeout evaluated    |
| Timeout Reached        | Light OFF            |
| Bright Environment     | Light OFF            |
| Manual Mode            | Manual state applied |

---

### 🌬️ Automatic Fan Control

The fan uses a temperature-based hysteresis mechanism to avoid unnecessary rapid switching around the threshold.

```text
              Read Temperature
                     ↓
            Temperature ≥ 28°C?
              /          \
            YES           NO
             │             │
             ▼             ▼
          FAN ON      Temperature ≤ 27°C?
                           /       \
                         YES        NO
                          │          │
                          ▼          ▼
                       FAN OFF   Keep Previous
                                  Fan State
```

### Fan Behaviour

| Temperature               | Fan Action              |
| ------------------------- | ----------------------- |
| ≥ 28°C                    | ON                      |
| ≤ 27°C                    | OFF                     |
| 27°C < Temperature < 28°C | Maintain previous state |

The hysteresis range helps prevent repeated ON/OFF switching when temperature fluctuates around the control boundary.

---

### 🕹️ Manual Appliance Control

Dedicated push buttons provide manual control for:

* Room light
* Fan
* Security system

Manual control allows the user to override automatic decisions when required.

```text
             User Button Press
                    ↓
             Manual Command
                    ↓
          Update Appliance State
                    ↓
        Manual State Overrides
          Automatic Decision
```

---

### 🔐 Security Monitoring

The security subsystem operates independently from normal appliance automation.

When security is armed, PIR motion detection becomes a security event.

```text
          Security System ARMED
                    ↓
             PIR Motion Detected
                    ↓
              Security Event
                    ↓
          ┌────────────────────┐
          │   Security LED ON  │
          │   Buzzer Activated │
          └────────────────────┘
                    ↓
             Alarm TRIGGERED
```

### Security Behaviour

| Security State | Motion       | System Response       |
| -------------- | ------------ | --------------------- |
| DISARMED       | Detected     | No security alarm     |
| ARMED          | Not Detected | Continue monitoring   |
| ARMED          | Detected     | Security LED + Buzzer |
| Disarmed       | Any          | Alarm cleared         |

---

### 🖥️ OLED Status Display

The SSD1306 OLED provides real-time information about:

* Temperature
* Humidity
* LDR value
* Motion state
* Room-light state
* Fan state
* Light mode
* Fan mode
* Security state
* Alarm state

---

### 📡 Serial Telemetry

The Serial Monitor provides structured information for:

* Debugging
* Sensor monitoring
* Automation verification
* State verification
* Security-event monitoring
* Functional testing

---

## 🔧 Components Used

| Component           |    Quantity | Purpose                          |
| ------------------- | ----------: | -------------------------------- |
| ESP32 DevKit        |           1 | Main microcontroller             |
| DHT22 Sensor        |           1 | Temperature and humidity sensing |
| LDR                 |           1 | Ambient-light measurement        |
| PIR Sensor          |           1 | Motion detection                 |
| SSD1306 OLED 128×64 |           1 | Real-time status display         |
| Room-Light LED      |           1 | Simulated room lighting          |
| Fan LED             |           1 | Simulated fan status             |
| Security LED        |           1 | Security alarm indication        |
| Safe-Status LED     |           1 | Normal security indication       |
| Buzzer              |           1 | Audible security alarm           |
| Push Buttons        |           3 | Light, fan, and security control |
| Resistors           | As required | Current limiting                 |
| Jumper Wires        | As required | Circuit connections              |
| Breadboard          |           1 | Prototype wiring                 |

---

## 🛠️ Technologies Used

| Technology        | Application                      |
| ----------------- | -------------------------------- |
| ESP32             | Main embedded controller         |
| Embedded C/C++    | Firmware development             |
| Arduino Framework | Firmware development             |
| DHT22             | Temperature and humidity sensing |
| LDR               | Ambient-light sensing            |
| PIR               | Motion detection                 |
| SSD1306 OLED      | System status interface          |
| I2C               | OLED communication               |
| GPIO              | Digital input/output             |
| ADC               | LDR measurement                  |
| Wokwi             | Virtual circuit simulation       |
| Serial Monitor    | Debugging and telemetry          |

---

## 🧠 Embedded Concepts Used

* **GPIO Programming** — sensor, button, LED and buzzer interfacing
* **ADC** — LDR ambient-light measurement
* **I2C Communication** — SSD1306 OLED interfacing
* **Digital Input** — PIR and push-button reading
* **Digital Output** — LED and buzzer control
* **Threshold Control** — lighting and fan decisions
* **Hysteresis** — stable temperature-based fan control
* **Timer Logic** — motion timeout handling
* **State Management** — automatic, manual, armed and alarm states
* **Event Detection** — motion and button events
* **Priority Control** — manual control over automatic operation
* **Serial Communication** — debugging and telemetry
* **Sensor Interfacing** — DHT22, LDR and PIR
* **Simulation Testing** — functional validation using Wokwi

---

## 🏗️ Architecture

```text
                         ┌───────────────────────┐
                         │      ESP32 DevKit     │
                         │    Main Controller    │
                         └───────────┬───────────┘
                                     │
              ┌──────────────────────┼──────────────────────┐
              │                      │                      │
              ▼                      ▼                      ▼
       ┌──────────────┐      ┌────────────────┐      ┌──────────────┐
       │ DHT22 Sensor │      │   LDR + PIR    │      │ Push Buttons │
       │ Temp/Humidity│      │ Light + Motion │      │ User Control │
       └──────┬───────┘      └───────┬────────┘      └──────┬───────┘
              │                      │                      │
              └──────────────────────┼──────────────────────┘
                                     ▼
                            ┌────────────────────┐
                            │    Control Logic   │
                            │                    │
                            │ • Auto Lighting    │
                            │ • Fan Control      │
                            │ • Manual Override  │
                            │ • Security Logic   │
                            │ • State Management │
                            └─────────┬──────────┘
                                      │
                      ┌───────────────┼────────────────┐
                      │               │                │
                      ▼               ▼                ▼
               ┌──────────────┐ ┌───────────────┐ ┌──────────────┐
               │ SSD1306 OLED │ │ Light / Fan   │ │ Security     │
               │ Status       │ │ LED Actuators │ │ LED + Buzzer │
               └──────────────┘ └───────────────┘ └──────────────┘
                                      │
                                      ▼
                               ┌─────────────────┐
                               │ Serial Monitor  │
                               │ Live Telemetry  │
                               └─────────────────┘
```

### Control Flow

```text
Sensors + User Inputs
        ↓
ESP32 Input Processing
        ↓
Control & Decision Logic
        ↓
 ┌──────┼─────────┬──────────┐
 ↓      ↓         ↓          ↓
Light  Fan     Security     OLED
Control Control  Control    Display
 ↓      ↓         ↓          ↓
 └──────┴─────────┴──────────┘
              ↓
       Serial Telemetry
```

---

## 🔌 Circuit Connections

The following GPIO configuration corresponds to the current ESP32 firmware configuration used in the project.

| Module              | ESP32 GPIO | Interface      |
| ------------------- | ---------: | -------------- |
| DHT22 Data          |     GPIO 4 | Digital Input  |
| PIR Output          |    GPIO 27 | Digital Input  |
| LDR                 |    GPIO 34 | ADC Input      |
| Room-Light LED      |    GPIO 18 | Digital Output |
| Fan LED             |    GPIO 19 | Digital Output |
| Buzzer              |    GPIO 23 | Digital Output |
| Security LED        |    GPIO 25 | Digital Output |
| Safe-Status LED     |    GPIO 26 | Digital Output |
| Manual Light Button |    GPIO 32 | Digital Input  |
| Manual Fan Button   |    GPIO 33 | Digital Input  |
| Security Button     |    GPIO 14 | Digital Input  |
| OLED SDA            |    GPIO 21 | I2C            |
| OLED SCL            |    GPIO 22 | I2C            |

> **Important:** Always verify the final GPIO assignments against `sketch.ino` and `diagram.json` before physical deployment.

---

## ⚙️ Automation Logic

### 💡 Automatic Lighting Logic

```text
                   Read LDR
                      ↓
                 Is Room Dark?
                 /           \
               YES            NO
                │              │
                ▼              ▼
          Read PIR Motion    Light OFF
                │
              Motion?
              /    \
            YES     NO
             │       │
             ▼       ▼
         Light ON  Check Timeout
                       │
                       ▼
                   Light OFF
```

### Lighting Conditions

| Condition              | Result               |
| ---------------------- | -------------------- |
| Dark + Motion Detected | Light ON             |
| Dark + No Motion       | Timeout evaluated    |
| Timeout Reached        | Light OFF            |
| Bright Environment     | Light OFF            |
| Manual Mode            | Manual state applied |

---

## 🌬️ Fan Automation Logic

```text
                Read Temperature
                       ↓
               Temperature ≥ 28°C?
                  /           \
                YES            NO
                 │              │
                 ▼              ▼
              FAN ON       Temperature ≤ 27°C?
                                  /       \
                                YES        NO
                                 │          │
                                 ▼          ▼
                              FAN OFF   Keep Previous
                                         Fan State
```

### Fan Conditions

| Temperature     | Action                    |
| --------------- | ------------------------- |
| ≥ 28°C          | Fan ON                    |
| ≤ 27°C          | Fan OFF                   |
| 27°C < T < 28°C | Previous state maintained |

---

## 🕹️ Manual Override

Manual control allows the user to directly control appliances when automatic behaviour is not desired.

### Light Control

```text
Light Button Pressed
        ↓
Toggle Light State
        ↓
Manual Light Control
```

### Fan Control

```text
Fan Button Pressed
        ↓
Toggle Fan State
        ↓
Manual Fan Control
```

### Security Control

```text
Security Button Pressed
        ↓
Toggle Security State
        ↓
ARM / DISARM
```

### Control Priority

```text
User Command
     ↓
Manual Control
     ↓
Overrides Automatic Decision
```

This provides flexibility during testing, maintenance, and special operating conditions.

---

## 🔐 Security System

The security system is independent from normal appliance automation.

### Security State Flow

```text
Security Button
      ↓
  ARM / DISARM
      ↓
Security ARMED
      ↓
PIR Motion Detected
      ↓
 Security Event
      ↓
┌────────────────────────┐
│    Security LED ON      │
│    Buzzer Activated     │
└────────────────────────┘
      ↓
Alarm TRIGGERED
```

### Security Behaviour

| Security State | Motion       | System Response        |
| -------------- | ------------ | ---------------------- |
| DISARMED       | Detected     | No security alarm      |
| ARMED          | Not detected | Continue monitoring    |
| ARMED          | Detected     | LED + buzzer activated |
| Disarmed       | Any          | Alarm cleared          |

---

## 📁 Folder Structure

```text
Smart-Home-Controller-Embedded-System/
│
├── Output/
│   ├── 01_Normal-Idle-State(5).png
│   ├── 02_Manual-Lighting-Motion(5).png
│   ├── 03-automatic-fan-high-temperature(5).png
│   └── 04-security-alarm-motion-detected(5).png
│
├── diagram.json
├── libraries.txt
├── sketch.ino
└── README.md
```

### Repository Contents

| File / Folder   | Description                         |
| --------------- | ----------------------------------- |
| `sketch.ino`    | Complete ESP32 firmware             |
| `diagram.json`  | Wokwi circuit and wiring definition |
| `libraries.txt` | Required Arduino libraries          |
| `Output/`       | Simulation output screenshots       |
| `README.md`     | Complete project documentation      |

---

## ⚙️ Installation

### Prerequisites

* Arduino IDE
* ESP32 board package
* Required Arduino libraries
* Web browser for Wokwi simulation

### Required Libraries

```text
Wire
Adafruit GFX Library
Adafruit SSD1306
DHT sensor library
```

### Arduino IDE Setup

1. Install Arduino IDE.
2. Install ESP32 board support.
3. Install the required libraries.
4. Open `sketch.ino`.
5. Select the appropriate ESP32 board.
6. Verify the GPIO configuration.
7. Compile the project.
8. Upload the firmware to the ESP32.

---

## ▶️ How to Run

### Using Arduino IDE

1. Connect the ESP32 development board.
2. Open `sketch.ino`.
3. Select the correct ESP32 board.
4. Select the correct COM port.
5. Compile the firmware.
6. Upload the program.
7. Open the Serial Monitor.
8. Set the baud rate according to the firmware configuration.
9. Monitor sensor readings and system states.

### Expected Operation

After initialization, the ESP32 continuously performs:

```text
Read Sensors
     ↓
Process Inputs
     ↓
Apply Control Logic
     ↓
Update Appliances
     ↓
Update Security
     ↓
Update OLED
     ↓
Send Serial Telemetry
     ↓
Repeat
```

---

## 🖥️ Wokwi Simulation

The complete prototype was designed and functionally validated in the **Wokwi virtual simulation environment**.

### Simulated Components

* ESP32 DevKit
* DHT22 sensor
* PIR motion sensor
* LDR
* SSD1306 OLED
* Room-light LED
* Fan LED
* Security LED
* Safe-status LED
* Buzzer
* Push buttons

### Simulation Procedure

1. Open the Wokwi project.
2. Load `diagram.json`.
3. Open the ESP32 firmware.
4. Start the simulation.
5. Observe the OLED/status output.
6. Open the Serial Monitor.
7. Change temperature and environmental conditions.
8. Change the LDR condition.
9. Trigger PIR motion.
10. Press the manual control buttons.
11. Arm and disarm the security system.
12. Verify the corresponding system response.
13. Capture the output for each major operating scenario.

---

## 🧪 Test Scenarios

The system was evaluated across the major operating conditions demonstrated by the simulation.

| Test ID | Scenario           | Input / Condition              | Expected Behaviour                       |
| ------- | ------------------ | ------------------------------ | ---------------------------------------- |
| T01     | Normal / Idle      | Normal temperature, no motion  | Light and fan remain OFF                 |
| T02     | Manual Lighting    | Manual light command           | Room light turns ON                      |
| T03     | Motion Detection   | PIR detects motion             | Motion state becomes DETECTED            |
| T04     | Dark Environment   | Low LDR reading                | Lighting logic evaluates room as dark    |
| T05     | High Temperature   | Temperature ≥ 28°C             | Fan turns ON                             |
| T06     | Low Temperature    | Temperature ≤ 27°C             | Fan turns OFF                            |
| T07     | Fan Hysteresis     | 27°C < T < 28°C                | Previous fan state maintained            |
| T08     | Manual Fan         | Fan button pressed             | Fan state toggles                        |
| T09     | Security Arm       | Security button pressed        | Security becomes ARMED                   |
| T10     | Security Intrusion | PIR detects motion while armed | Alarm is triggered                       |
| T11     | Security Disarm    | Security button pressed        | Security becomes DISARMED                |
| T12     | OLED Monitoring    | System operating               | Live status displayed                    |
| T13     | Serial Monitoring  | System operating               | Telemetry displayed                      |
| T14     | Combined Operation | Multiple conditions active     | Automation and security operate together |

---

## 📟 Simulation Output

The following four operating states were captured during the Wokwi simulation.

### 01. Normal / Idle State

```text
========================================
       SMART HOME CONTROLLER
========================================

Temperature : 27.0 C
Humidity    : 55.0 %
LDR Value   : 1001
Room Light  : DARK
Motion      : NO MOTION

ACTUATORS
----------------------------------------
Room Light  : OFF
Fan         : OFF
Light Mode  : AUTO
Fan Mode    : AUTO

SECURITY
----------------------------------------
Security    : DISARMED
Alarm       : SAFE
```

**Demonstrates:**

* Normal environmental readings
* No motion detected
* Room light OFF
* Fan OFF
* Automatic operating modes
* Security DISARMED
* Alarm SAFE

---

### 02. Manual Lighting + Motion

```text
SYSTEM STATUS
----------------------------------------

Temperature : 27.0 C
Humidity    : 55.0 %
LDR Value   : 32
Room Light  : DARK
Motion      : DETECTED

ACTUATORS
----------------------------------------
Room Light  : ON
Fan         : OFF
Light Mode  : MANUAL
Fan Mode    : AUTO

SECURITY
----------------------------------------
Security    : DISARMED
Alarm       : SAFE
```

**Demonstrates:**

* Dark-room condition
* Motion detection
* Room-light activation
* Manual light control
* Fan remaining OFF
* Security subsystem remaining independent

---

### 03. Automatic Fan – High Temperature

```text
SYSTEM STATUS
----------------------------------------

Temperature : 35.0 C
Humidity    : 55.0 %
LDR Value   : 32
Room Light  : DARK
Motion      : NO MOTION

ACTUATORS
----------------------------------------
Room Light  : OFF
Fan         : ON
Light Mode  : AUTO
Fan Mode    : AUTO

SECURITY
----------------------------------------
Security    : DISARMED
Alarm       : SAFE
```

**Demonstrates:**

* High-temperature condition
* Automatic temperature-based fan activation
* Fan operating in AUTO mode
* Room light remaining OFF
* Security remaining SAFE

---

### 04. Security Alarm – Motion Detected

```text
SYSTEM STATUS
----------------------------------------

Temperature : 35.0 C
Humidity    : 55.0 %
LDR Value   : 32
Room Light  : DARK
Motion      : DETECTED

ACTUATORS
----------------------------------------
Room Light  : ON
Fan         : ON
Light Mode  : AUTO
Fan Mode    : AUTO

SECURITY
----------------------------------------
Security    : ARMED
Alarm       : TRIGGERED
```

**Demonstrates:**

* Security system ARMED
* PIR motion detection
* Security event detection
* Alarm TRIGGERED
* Simultaneous operation of automation and security functions

---

## 📸 Screenshots

The following screenshots document the **four major functional scenarios** demonstrated during the Wokwi simulation.

### 01. Normal / Idle State

**Scenario demonstrated:**

* Normal environmental conditions
* No motion detected
* Room light OFF
* Fan OFF
* Automatic operating modes
* Security DISARMED
* Alarm SAFE

![Normal Idle State](Output/01_Normal-Idle-State%285%29.png)

---

### 02. Manual Lighting + Motion Detection

**Scenario demonstrated:**

* Dark-room condition
* Motion detected
* Room light ON
* Manual light mode
* Fan OFF
* Security remains DISARMED

![Manual Lighting and Motion](Output/02_Manual-Lighting-Motion%285%29.png)

---

### 03. Automatic Fan – High Temperature

**Scenario demonstrated:**

* Temperature reaches a high value
* Automatic temperature-based fan control
* Fan ON
* Fan mode AUTO
* Room light OFF
* Security remains SAFE

![Automatic Fan High Temperature](Output/03-automatic-fan-high-temperature%285%29.png)

---

### 04. Security Alarm – Motion Detected

**Scenario demonstrated:**

* Security system ARMED
* PIR motion detected
* Alarm TRIGGERED
* Security indication activated
* Simultaneous embedded-system operation

![Security Alarm Motion Detected](Output/04-security-alarm-motion-detected%285%29.png)

---

## 📊 Test Results

| Function                     | Result |
| ---------------------------- | ------ |
| DHT22 Temperature Monitoring | ✅ PASS |
| DHT22 Humidity Monitoring    | ✅ PASS |
| LDR Ambient-Light Monitoring | ✅ PASS |
| PIR Motion Detection         | ✅ PASS |
| Automatic Lighting           | ✅ PASS |
| Automatic Fan Control        | ✅ PASS |
| Temperature Hysteresis       | ✅ PASS |
| Manual Light Control         | ✅ PASS |
| Manual Fan Control           | ✅ PASS |
| Security Arm/Disarm          | ✅ PASS |
| Security Alarm Indication    | ✅ PASS |
| OLED Status Display          | ✅ PASS |
| Serial Telemetry             | ✅ PASS |
| Wokwi Functional Simulation  | ✅ PASS |

### Overall System Result

```text
Multiple Sensors
       +
User Inputs
       +
Embedded Control Logic
       +
Actuator Control
       +
Security Monitoring
       +
Real-Time Feedback
       ↓
ESP32-Based Smart Home Controller
```

The project demonstrates the integration of **multiple sensors, user inputs, control algorithms, actuator outputs, security monitoring, and real-time feedback** within a single embedded firmware application.

---

## ⚠️ Known Limitations

The current prototype has the following limitations:

* The system has primarily been validated through Wokwi simulation.
* LEDs are used to represent real appliances.
* Physical appliance control requires appropriately rated switching hardware.
* Mains-powered loads require proper electrical isolation and safety protection.
* PIR provides motion detection but does not identify individuals.
* LDR readings represent relative brightness rather than calibrated lux measurements.
* Wi-Fi connectivity is not currently implemented.
* Cloud integration is not currently implemented.
* Persistent event logging is not currently implemented.
* A dedicated mobile or web dashboard is not currently included.
* The current prototype represents a single-room automation architecture.

---

## 🚀 Future Improvements

### 🌐 IoT Connectivity

* Add Wi-Fi-based remote monitoring.
* Implement MQTT communication.
* Develop a web dashboard.
* Develop a mobile monitoring interface.
* Add remote appliance control.

### ⚡ Energy Management

* Add current and voltage sensors.
* Measure real-time appliance energy consumption.
* Generate energy-usage reports.
* Implement energy-optimization strategies.
* Add appliance-level power monitoring.

### 🔐 Security Enhancement

* Add door and window sensors.
* Add additional PIR sensors.
* Add remote security notifications.
* Implement event logging.
* Add timestamp-based security records.
* Add camera-based verification.

### ⚙️ Embedded-System Improvements

* Add non-volatile configuration storage.
* Make temperature thresholds configurable.
* Make motion timeout configurable.
* Add OTA firmware updates.
* Add watchdog-based fault recovery.
* Replace simulated LEDs with properly isolated appliance drivers.

### 📱 User Interface

* Add a web-based dashboard.
* Add smartphone control.
* Display historical sensor data.
* Provide real-time notifications.
* Add configuration controls for thresholds and operating modes.

### 📈 Scalability

The current single-room architecture can be extended into a larger automation platform:

```text
Single Room
     ↓
Multi-Room Automation
     ↓
Smart Building
     ↓
IoT-Based Building Management System
```

---

## 🎓 Learning Outcomes

This project provided practical experience in:

* ESP32 microcontroller programming
* Embedded C/C++ development
* Sensor interfacing
* GPIO configuration
* ADC-based sensing
* I2C communication
* OLED display programming
* Digital input handling
* Digital output control
* Temperature-based automation
* Hysteresis implementation
* Manual override design
* Security event detection
* State management
* Timer-based control
* Serial debugging
* Embedded-system testing
* Wokwi simulation
* Technical documentation

### Core Engineering Takeaway

```text
Sensors
   +
User Inputs
   +
Microcontroller
   +
Control Algorithms
   +
Actuators
   +
Security Logic
   +
User Feedback
   ↓
Complete Embedded System
```

The project demonstrates practical capability in **embedded firmware development, hardware interfacing, sensor-driven automation, state-based control, security monitoring, and simulation-based system validation**.

---

## 👤 Author

**Subham Bhattacherjee**

**M.Tech Computer Science & Engineering**

### Project

**Smart Home Controller – Embedded Systems**

### Technical Focus

* Embedded Systems
* ESP32
* C/C++
* IoT
* Sensor Interfacing
* Automation
* Firmware Development
* Wokwi Simulation
* Security Systems
* Real-Time Monitoring

---

## 📜 License

This project is licensed under the **MIT License**.

You are free to use, modify, and distribute this project with appropriate attribution.

---

## ⭐ Project Summary

**Smart Home Controller** is an ESP32-based embedded automation and security prototype that integrates **environmental sensing, automatic lighting, temperature-based fan control, manual override, motion detection, security monitoring, OLED visualization, and Serial telemetry** into a single embedded system.

The project demonstrates practical embedded-system concepts including:

* Sensor interfacing
* ADC
* GPIO
* I2C communication
* Threshold-based automation
* Temperature hysteresis
* State management
* Manual override
* Event detection
* Security monitoring
* Real-time system feedback
* Simulation-based validation

The complete prototype was developed and functionally validated using **Wokwi**, providing a foundation for future expansion toward **IoT-enabled smart homes, smart buildings, energy management, and connected security systems**.

---

## 📌 Recruiter-Focused Project Highlights

**What this project demonstrates:**

* Built an **ESP32-based embedded automation system** from sensor input to actuator output.
* Integrated **DHT22, LDR, PIR, OLED, LEDs, buzzer, and push buttons**.
* Implemented **automatic lighting using environmental and motion inputs**.
* Implemented **temperature-based fan automation with hysteresis**.
* Developed **manual override functionality** for appliance control.
* Designed an **independent PIR-based security monitoring subsystem**.
* Implemented **visual and audible alarm indication**.
* Used **I2C communication** for OLED interfacing.
* Used **ADC-based sensing** for ambient-light measurement.
* Implemented **state-based embedded control logic**.
* Validated multiple real-world operating scenarios using **Wokwi simulation**.
* Documented the system using **four functional simulation screenshots**.
* Structured the project using `sketch.ino`, `diagram.json`, `libraries.txt`, and `Output/`.
* Demonstrated practical understanding of **embedded firmware, sensor interfacing, automation, security, and real-time monitoring**.

> **Project Type:** Embedded Systems / IoT / Smart Home Automation
> **Platform:** ESP32
> **Programming:** Embedded C/C++
> **Simulation:** Wokwi
> **Core Areas:** Sensor Interfacing, Automation, Firmware, Security, Real-Time Monitoring
