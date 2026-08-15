#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ============================================================
// PROJECT CONFIGURATION
// ============================================================

#define TOTAL_SLOTS 4

const float OCCUPIED_THRESHOLD_CM = 35.0f;

#define SAMPLE_COUNT 5
#define ECHO_TIMEOUT_US 60000UL

const unsigned long SENSOR_SAMPLE_INTERVAL_MS = 60;
const unsigned long STATE_DEBOUNCE_MS = 800;
#define REQUIRED_CONSECUTIVE_DECISIONS 2

const unsigned long OLED_UPDATE_INTERVAL_MS = 500;
const unsigned long SERIAL_UPDATE_INTERVAL_MS = 1000;

// ============================================================
// WIFI
// ============================================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

WebServer server(80);

bool wifiStarted = false;
unsigned long lastWiFiRetry = 0;
const unsigned long WIFI_RETRY_INTERVAL_MS = 10000;

// ============================================================
// OLED
// IMPORTANT: matches diagram.json -> SDA 32, SCL 23
// ============================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 32
#define OLED_SCL 23
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool oledOK = false;

// ============================================================
// SERVO
// GPIO 15
// ============================================================

#define SERVO_PIN 15
#define SERVO_FREQUENCY 50
#define SERVO_RESOLUTION 14

#define SERVO_CLOSED_ANGLE 0
#define SERVO_OPEN_ANGLE 90

const unsigned long GATE_OPEN_TIME_MS = 2000;
const unsigned long GATE_CYCLE_INTERVAL_MS = 5000;

bool gateOpen = false;
unsigned long gateOpenedAt = 0;
unsigned long lastGateCycle = 0;

// ============================================================
// BUZZER
// GPIO 0 - matches diagram.json
// ============================================================

#define BUZZER_PIN 0

const unsigned long FULL_ALERT_INTERVAL_MS = 3000;
unsigned long lastFullAlert = 0;

enum BuzzerState {
  BUZZER_IDLE,
  BUZZER_BEEP_1,
  BUZZER_GAP,
  BUZZER_BEEP_2
};

BuzzerState buzzerState = BUZZER_IDLE;
unsigned long buzzerStateStarted = 0;

// ============================================================
// ULTRASONIC SENSOR PIN MAP
// ============================================================

struct UltrasonicSensor {
  uint8_t trigPin;
  uint8_t echoPin;
};

UltrasonicSensor sensors[TOTAL_SLOTS] = {
  {5, 17},    // Slot 1
  {16, 4},    // Slot 2
  {27, 26},   // Slot 3
  {25, 33}    // Slot 4
};

// ============================================================
// LED PIN MAP
// IMPORTANT: matches diagram.json
//
// Slot 1: Green 18, Red 2
// Slot 2: Green 19, Red 12
// Slot 3: Green 21, Red 13
// Slot 4: Green 22, Red 14
//
// OLED uses 32/23
// Servo uses 15
// Buzzer uses 0
// ============================================================

struct LedPair {
  uint8_t greenPin;
  uint8_t redPin;
};

LedPair slotLEDs[TOTAL_SLOTS] = {
  {18, 2},
  {19, 12},
  {21, 13},
  {22, 14}
};

// ============================================================
// SLOT STATE
// ============================================================

struct SlotState {
  bool occupied;
  bool sensorFault;

  bool candidateState;
  bool candidatePending;
  uint8_t consecutiveDecisions;
  unsigned long candidateStartedAt;

  float lastDistance;
  bool lastReadingValid;
  uint8_t invalidSamples;
};

SlotState slots[TOTAL_SLOTS];

// ============================================================
// SENSOR SCAN STATE
// ============================================================

float sensorSamples[SAMPLE_COUNT];

uint8_t currentSensor = 0;
uint8_t currentSample = 0;
unsigned long nextSensorReadAt = 0;

// ============================================================
// TIMERS
// ============================================================

unsigned long lastOLEDUpdate = 0;
unsigned long lastSerialUpdate = 0;

// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

float readDistanceCM(uint8_t sensorIndex, bool &valid);

float calculateFilteredDistance(
  uint8_t sensorIndex,
  bool &sensorFault
);

void processSensorDecision(
  uint8_t sensorIndex,
  float averageDistance,
  bool sensorFault
);

void updateSlotLEDs(
  uint8_t slotIndex,
  bool occupied,
  bool sensorFault
);

int getOccupiedCount();
int getFaultCount();
int getAvailableCount();

void setServoAngle(int angle);
void updateGate();

void startFullAlert();
void updateBuzzer();

void updateOLED();
void printSerialStatus();

void connectWiFi();
void handleRoot();
String buildWebPage();

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("================================================");
  Serial.println(" SMART PARKING SYSTEM");
  Serial.println(" FINAL INDUSTRY-ORIENTED FIRMWARE");
  Serial.println(" ESP32 + 4 ULTRASONIC SENSORS");
  Serial.println("================================================");

  // ----------------------------------------------------------
  // Sensors and LEDs
  // ----------------------------------------------------------

  for (int i = 0; i < TOTAL_SLOTS; i++) {

    pinMode(sensors[i].trigPin, OUTPUT);
    pinMode(sensors[i].echoPin, INPUT);

    digitalWrite(sensors[i].trigPin, LOW);

    pinMode(slotLEDs[i].greenPin, OUTPUT);
    pinMode(slotLEDs[i].redPin, OUTPUT);

    slots[i].occupied = false;
    slots[i].sensorFault = false;

    slots[i].candidateState = false;
    slots[i].candidatePending = false;
    slots[i].consecutiveDecisions = 0;
    slots[i].candidateStartedAt = 0;

    slots[i].lastDistance = 100.0f;
    slots[i].lastReadingValid = true;
    slots[i].invalidSamples = 0;

    updateSlotLEDs(i, false, false);
  }

  // ----------------------------------------------------------
  // Buzzer
  // ----------------------------------------------------------

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  // ----------------------------------------------------------
  // OLED
  // ----------------------------------------------------------

  Wire.begin(OLED_SDA, OLED_SCL);

  oledOK = display.begin(
    SSD1306_SWITCHCAPVCC,
    OLED_ADDRESS
  );

  if (oledOK) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("SMART PARKING");
    display.println();
    display.println("System Starting...");
    display.display();
    delay(500);
  } else {
    Serial.println("OLED ERROR: SSD1306 not detected.");
  }

  // ----------------------------------------------------------
  // Servo
  // ----------------------------------------------------------

  ledcAttach(
    SERVO_PIN,
    SERVO_FREQUENCY,
    SERVO_RESOLUTION
  );

  setServoAngle(SERVO_CLOSED_ANGLE);
  gateOpen = false;

  // ----------------------------------------------------------
  // WiFi + Web Server
  // ----------------------------------------------------------

  connectWiFi();

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Web server started on port 80.");

  updateOLED();
  printSerialStatus();

  Serial.println("SYSTEM READY");
  Serial.println("================================================");
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  unsigned long now = millis();

  // Web server must be serviced continuously.
  server.handleClient();

  // ----------------------------------------------------------
  // Sequential ultrasonic measurement
  // ----------------------------------------------------------

  if (now >= nextSensorReadAt) {

    bool validReading = false;

    float distance =
      readDistanceCM(
        currentSensor,
        validReading
      );

    sensorSamples[currentSample] = distance;
    currentSample++;

    if (currentSample >= SAMPLE_COUNT) {

      bool sensorFault = false;

      float filteredDistance =
        calculateFilteredDistance(
          currentSensor,
          sensorFault
        );

      slots[currentSensor].lastDistance =
        filteredDistance;

      slots[currentSensor].lastReadingValid =
        !sensorFault;

      processSensorDecision(
        currentSensor,
        filteredDistance,
        sensorFault
      );

      currentSample = 0;

      currentSensor++;

      if (currentSensor >= TOTAL_SLOTS) {
        currentSensor = 0;
      }
    }

    nextSensorReadAt =
      millis() + SENSOR_SAMPLE_INTERVAL_MS;
  }

  // ----------------------------------------------------------
  // OLED
  // ----------------------------------------------------------

  if (
    now - lastOLEDUpdate >=
    OLED_UPDATE_INTERVAL_MS
  ) {
    updateOLED();
    lastOLEDUpdate = now;
  }

  // ----------------------------------------------------------
  // Serial
  // ----------------------------------------------------------

  if (
    now - lastSerialUpdate >=
    SERIAL_UPDATE_INTERVAL_MS
  ) {
    printSerialStatus();
    lastSerialUpdate = now;
  }

  // ----------------------------------------------------------
  // Gate and buzzer
  // ----------------------------------------------------------

  updateGate();
  updateBuzzer();

  // ----------------------------------------------------------
  // Non-blocking WiFi recovery
  // ----------------------------------------------------------

  if (
    WiFi.status() != WL_CONNECTED &&
    now - lastWiFiRetry >= WIFI_RETRY_INTERVAL_MS
  ) {
    lastWiFiRetry = now;
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
}

// ============================================================
// ULTRASONIC DISTANCE READING
// ============================================================

float readDistanceCM(
  uint8_t sensorIndex,
  bool &valid
) {

  valid = false;

  uint8_t trig =
    sensors[sensorIndex].trigPin;

  uint8_t echo =
    sensors[sensorIndex].echoPin;

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);

  unsigned long echoTime =
    pulseIn(
      echo,
      HIGH,
      ECHO_TIMEOUT_US
    );

  if (echoTime == 0) {
    return 400.0f;
  }

  float distance =
    (echoTime * 0.0343f) / 2.0f;

  if (
    distance >= 2.0f &&
    distance <= 400.0f
  ) {
    valid = true;
    return distance;
  }

  return 400.0f;
}

// ============================================================
// 5-SAMPLE FILTER
//
// Required method:
// 5 readings -> remove highest -> remove lowest
// -> average remaining 3.
//
// Industry addition:
// If 2 or more readings are invalid, declare sensor fault.
// This prevents a faulty sensor from falsely reporting FREE.
// ============================================================

float calculateFilteredDistance(
  uint8_t sensorIndex,
  bool &sensorFault
) {

  float minimum = sensorSamples[0];
  float maximum = sensorSamples[0];

  float sum = 0.0f;

  uint8_t invalidCount = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {

    float value = sensorSamples[i];

    sum += value;

    if (value < minimum) {
      minimum = value;
    }

    if (value > maximum) {
      maximum = value;
    }

    if (value >= 400.0f) {
      invalidCount++;
    }
  }

  slots[sensorIndex].invalidSamples =
    invalidCount;

  // Fail-safe sensor validation.
  sensorFault = (invalidCount >= 2);

  if (sensorFault) {
    return 400.0f;
  }

  // Remove highest and lowest.
  sum -= minimum;
  sum -= maximum;

  return sum / 3.0f;
}

// ============================================================
// SLOT DECISION + DEBOUNCE
// ============================================================

void processSensorDecision(
  uint8_t sensorIndex,
  float averageDistance,
  bool sensorFault
) {

  SlotState &slot =
    slots[sensorIndex];

  // ----------------------------------------------------------
  // SENSOR FAULT
  // ----------------------------------------------------------

  if (sensorFault) {

    slot.sensorFault = true;

    slot.candidatePending = false;
    slot.consecutiveDecisions = 0;

    // Fail-safe indication:
    // red LED on, green LED off.
    updateSlotLEDs(
      sensorIndex,
      true,
      true
    );

    return;
  }

  // ----------------------------------------------------------
  // VALID SENSOR
  // ----------------------------------------------------------

  slot.sensorFault = false;
  slot.lastReadingValid = true;

  bool decision =
    (averageDistance < OCCUPIED_THRESHOLD_CM);

  // State already matches.
  if (decision == slot.occupied) {

    slot.candidatePending = false;
    slot.consecutiveDecisions = 0;

    updateSlotLEDs(
      sensorIndex,
      slot.occupied,
      false
    );

    return;
  }

  // New candidate state.
  if (
    !slot.candidatePending ||
    slot.candidateState != decision
  ) {

    slot.candidatePending = true;
    slot.candidateState = decision;

    slot.consecutiveDecisions = 1;

    slot.candidateStartedAt =
      millis();

    return;
  }

  // Count consecutive decisions.
  if (
    slot.consecutiveDecisions <
    REQUIRED_CONSECUTIVE_DECISIONS
  ) {

    slot.consecutiveDecisions++;
  }

  // Apply state only after stability requirements.
  if (
    slot.consecutiveDecisions >=
      REQUIRED_CONSECUTIVE_DECISIONS &&

    millis() -
      slot.candidateStartedAt >=
      STATE_DEBOUNCE_MS
  ) {

    slot.occupied = decision;

    updateSlotLEDs(
      sensorIndex,
      slot.occupied,
      false
    );

    Serial.print("STATE CHANGE - Slot ");
    Serial.print(sensorIndex + 1);
    Serial.print(": ");

    Serial.println(
      slot.occupied ?
      "OCCUPIED" :
      "FREE"
    );

    slot.candidatePending = false;
    slot.consecutiveDecisions = 0;
  }
}

// ============================================================
// LED CONTROL
// ============================================================

void updateSlotLEDs(
  uint8_t slotIndex,
  bool occupied,
  bool sensorFault
) {

  if (sensorFault) {

    // Fail-safe:
    // red ON, green OFF.
    digitalWrite(
      slotLEDs[slotIndex].greenPin,
      LOW
    );

    digitalWrite(
      slotLEDs[slotIndex].redPin,
      HIGH
    );

    return;
  }

  if (occupied) {

    digitalWrite(
      slotLEDs[slotIndex].greenPin,
      LOW
    );

    digitalWrite(
      slotLEDs[slotIndex].redPin,
      HIGH
    );

  } else {

    digitalWrite(
      slotLEDs[slotIndex].greenPin,
      HIGH
    );

    digitalWrite(
      slotLEDs[slotIndex].redPin,
      LOW
    );
  }
}

// ============================================================
// PARKING COUNTERS
// ============================================================

int getOccupiedCount() {

  int occupied = 0;

  for (int i = 0; i < TOTAL_SLOTS; i++) {

    if (slots[i].occupied) {
      occupied++;
    }
  }

  return occupied;
}

int getFaultCount() {

  int faults = 0;

  for (int i = 0; i < TOTAL_SLOTS; i++) {

    if (slots[i].sensorFault) {
      faults++;
    }
  }

  return faults;
}

int getAvailableCount() {

  // Conservative/fail-safe:
  // faulty slots are not advertised as available.
  return TOTAL_SLOTS
         - getOccupiedCount()
         - getFaultCount();
}

// ============================================================
// SERVO
// ============================================================

void setServoAngle(int angle) {

  angle =
    constrain(angle, 0, 180);

  float pulseWidthUs =
    500.0f +
    ((float)angle / 180.0f) *
    2000.0f;

  float periodUs =
    1000000.0f /
    SERVO_FREQUENCY;

  uint32_t maxDuty =
    (1UL << SERVO_RESOLUTION) - 1;

  uint32_t duty =
    (uint32_t)(
      (pulseWidthUs / periodUs) *
      maxDuty
    );

  ledcWrite(
    SERVO_PIN,
    duty
  );
}

// ============================================================
// GATE CONTROL
//
// Demonstration mode:
// When capacity exists, the gate cycles open briefly.
// When full, it remains closed.
//
// NOTE:
// A true production entry gate requires a dedicated
// entrance/vehicle-detection sensor. This project currently
// has four slot sensors, so this is availability-based
// barrier demonstration logic.
// ============================================================

void updateGate() {

  unsigned long now = millis();

  int available =
    getAvailableCount();

  // Parking full -> gate closed.
  if (available <= 0) {

    if (gateOpen) {

      setServoAngle(
        SERVO_CLOSED_ANGLE
      );

      gateOpen = false;

      Serial.println(
        "GATE: CLOSED - PARKING FULL"
      );
    }

    return;
  }

  // Close gate after open interval.
  if (gateOpen) {

    if (
      now - gateOpenedAt >=
      GATE_OPEN_TIME_MS
    ) {

      setServoAngle(
        SERVO_CLOSED_ANGLE
      );

      gateOpen = false;

      Serial.println(
        "GATE: CLOSED"
      );
    }

    return;
  }

  // Demonstration opening cycle.
  if (
    now - lastGateCycle >=
    GATE_CYCLE_INTERVAL_MS
  ) {

    setServoAngle(
      SERVO_OPEN_ANGLE
    );

    gateOpen = true;

    gateOpenedAt = now;
    lastGateCycle = now;

    Serial.println(
      "GATE: OPEN - SPACE AVAILABLE"
    );
  }
}

// ============================================================
// BUZZER
// ============================================================

void startFullAlert() {

  tone(
    BUZZER_PIN,
    2000
  );

  buzzerState =
    BUZZER_BEEP_1;

  buzzerStateStarted =
    millis();

  lastFullAlert =
    millis();

  Serial.println(
    "PARKING FULL - BUZZER ALERT"
  );
}

void updateBuzzer() {

  unsigned long now =
    millis();

  int available =
    getAvailableCount();

  // Parking not full.
  if (available > 0) {

    noTone(BUZZER_PIN);

    buzzerState =
      BUZZER_IDLE;

    return;
  }

  // Start periodic double beep.
  if (
    buzzerState == BUZZER_IDLE &&
    now - lastFullAlert >=
      FULL_ALERT_INTERVAL_MS
  ) {

    startFullAlert();

    return;
  }

  // End first beep.
  if (
    buzzerState == BUZZER_BEEP_1 &&
    now - buzzerStateStarted >= 150
  ) {

    noTone(BUZZER_PIN);

    buzzerState =
      BUZZER_GAP;

    buzzerStateStarted =
      now;

    return;
  }

  // Start second beep.
  if (
    buzzerState == BUZZER_GAP &&
    now - buzzerStateStarted >= 200
  ) {

    tone(
      BUZZER_PIN,
      2000
    );

    buzzerState =
      BUZZER_BEEP_2;

    buzzerStateStarted =
      now;

    return;
  }

  // End second beep.
  if (
    buzzerState == BUZZER_BEEP_2 &&
    now - buzzerStateStarted >= 150
  ) {

    noTone(BUZZER_PIN);

    buzzerState =
      BUZZER_IDLE;
  }
}

// ============================================================
// OLED
// ============================================================

void updateOLED() {

  if (!oledOK) {
    return;
  }

  int occupied =
    getOccupiedCount();

  int faults =
    getFaultCount();

  int available =
    getAvailableCount();

  display.clearDisplay();

  display.setTextColor(
    SSD1306_WHITE
  );

  display.setTextSize(1);

  // Header
  display.setCursor(0, 0);

  if (available <= 0) {

    display.println(
      "** PARKING FULL **"
    );

  } else {

    display.print(
      "Available: "
    );

    display.print(
      available
    );

    display.println(
      "/4"
    );
  }

  // Slot states
  display.setCursor(0, 16);

  display.print("S1:");
  if (slots[0].sensorFault)
    display.print("X");
  else
    display.print(
      slots[0].occupied ? "O" : "F"
    );

  display.print("  S2:");
  if (slots[1].sensorFault)
    display.print("X");
  else
    display.print(
      slots[1].occupied ? "O" : "F"
    );

  display.setCursor(0, 28);

  display.print("S3:");
  if (slots[2].sensorFault)
    display.print("X");
  else
    display.print(
      slots[2].occupied ? "O" : "F"
    );

  display.print("  S4:");
  if (slots[3].sensorFault)
    display.print("X");
  else
    display.print(
      slots[3].occupied ? "O" : "F"
    );

  display.setCursor(0, 40);

  display.print(
    "Occ:"
  );
  display.print(
    occupied
  );

  display.print(
    " Free:"
  );
  display.print(
    available
  );

  if (faults > 0) {

    display.print(
      " F:"
    );
    display.print(
      faults
    );
  }

  display.setCursor(0, 52);

  display.print(
    "Gate:"
  );

  if (available <= 0) {

    display.print(
      "CLOSED"
    );

  } else if (gateOpen) {

    display.print(
      "OPEN"
    );

  } else {

    display.print(
      "READY"
    );
  }

  display.display();
}

// ============================================================
// SERIAL MONITOR
// ============================================================

void printSerialStatus() {

  Serial.println();
  Serial.println(
    "================================================"
  );

  for (int i = 0; i < TOTAL_SLOTS; i++) {

    Serial.print(
      "Slot "
    );

    Serial.print(
      i + 1
    );

    Serial.print(
      " | Distance: "
    );

    if (slots[i].sensorFault) {

      Serial.print(
        "INVALID"
      );

    } else {

      Serial.print(
        slots[i].lastDistance,
        1
      );

      Serial.print(
        " cm"
      );
    }

    Serial.print(
      " | State: "
    );

    if (slots[i].sensorFault) {

      Serial.println(
        "SENSOR FAULT"
      );

    } else {

      Serial.println(
        slots[i].occupied ?
        "OCCUPIED" :
        "FREE"
      );
    }
  }

  Serial.print(
    "Occupied: "
  );

  Serial.print(
    getOccupiedCount()
  );

  Serial.print(
    " | Faults: "
  );

  Serial.print(
    getFaultCount()
  );

  Serial.print(
    " | Available: "
  );

  Serial.println(
    getAvailableCount()
  );

  Serial.print(
    "Gate: "
  );

  Serial.println(
    gateOpen ?
    "OPEN" :
    "CLOSED"
  );

  if (WiFi.status() == WL_CONNECTED) {

    Serial.print(
      "Web Dashboard: http://"
    );

    Serial.println(
      WiFi.localIP()
    );
  }

  Serial.println(
    "================================================"
  );
}

// ============================================================
// WIFI
// ============================================================

void connectWiFi() {

  Serial.print(
    "Connecting to Wokwi-GUEST"
  );

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD,
    6
  );

  unsigned long start =
    millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - start < 10000
  ) {

    delay(250);
    Serial.print(".");
  }

  Serial.println();

  if (
    WiFi.status() ==
    WL_CONNECTED
  ) {

    wifiStarted = true;

    Serial.println(
      "WiFi connected!"
    );

    Serial.print(
      "IP Address: "
    );

    Serial.println(
      WiFi.localIP()
    );

  } else {

    wifiStarted = false;

    Serial.println(
      "WiFi timeout. Local system continues."
    );
  }
}

// ============================================================
// WEB DASHBOARD
// ============================================================

String buildWebPage() {

  int occupied =
    getOccupiedCount();

  int faults =
    getFaultCount();

  int available =
    getAvailableCount();

  String html;

  html.reserve(5000);

  html +=
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta charset='UTF-8'>"
    "<meta name='viewport' "
    "content='width=device-width,initial-scale=1'>"
    "<meta http-equiv='refresh' content='3'>"
    "<title>Smart Parking System</title>";

  html +=
    "<style>"
    "body{font-family:Arial;margin:20px;background:#f5f5f5;}"
    ".card{background:white;padding:15px;margin:10px 0;"
    "border-radius:8px;border:1px solid #ddd;}"
    ".free{color:green;font-weight:bold;}"
    ".occ{color:red;font-weight:bold;}"
    ".fault{color:red;font-weight:bold;}"
    ".full{color:red;font-size:25px;font-weight:bold;}"
    "</style>";

  html +=
    "</head><body>";

  html +=
    "<h1>Smart Parking System</h1>";

  if (available <= 0) {

    html +=
      "<div class='card full'>"
      "PARKING FULL / NO SAFE SPACE AVAILABLE"
      "</div>";

  } else {

    html +=
      "<div class='card'>"
      "<h2>Available Slots: " +
      String(available) +
      " / 4</h2>"
      "</div>";
  }

  html +=
    "<div class='card'>"
    "<h3>Occupied: " +
    String(occupied) +
    " / 4</h3>";

  html +=
    "<h3>Sensor Faults: " +
    String(faults) +
    "</h3>";

  html +=
    "</div>";

  for (
    int i = 0;
    i < TOTAL_SLOTS;
    i++
  ) {

    html +=
      "<div class='card'>"
      "<strong>Slot " +
      String(i + 1) +
      "</strong>"
      " | Distance: ";

    if (
      slots[i].sensorFault
    ) {

      html +=
        "<span class='fault'>INVALID</span>";

      html +=
        " | Status: "
        "<span class='fault'>"
        "SENSOR FAULT"
        "</span>";

    } else {

      html +=
        String(
          slots[i].lastDistance,
          1
        );

      html +=
        " cm | Status: ";

      if (slots[i].occupied) {

        html +=
          "<span class='occ'>"
          "OCCUPIED"
          "</span>";

      } else {

        html +=
          "<span class='free'>"
          "FREE"
          "</span>";
      }
    }

    html +=
      "</div>";
  }

  html +=
    "<div class='card'>"
    "<h3>Gate: ";

  if (available <= 0) {

    html +=
      "<span class='occ'>CLOSED</span>";

  } else if (gateOpen) {

    html +=
      "<span class='free'>OPEN</span>";

  } else {

    html +=
      "READY";
  }

  html +=
    "</h3>"
    "<p>Automatic refresh: 3 seconds</p>"
    "</div>";

  html +=
    "</body></html>";

  return html;
}

// ============================================================
// WEB SERVER ROOT
// ============================================================

void handleRoot() {

  server.send(
    200,
    "text/html",
    buildWebPage()
  );
}