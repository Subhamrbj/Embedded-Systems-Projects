#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===========================================================================
// PIN DEFINITIONS
// ===========================================================================

#define SENSOR_PIN       A0
#define BUZZER_PIN       11
#define GREEN_LED_PIN    12
#define RED_LED_PIN      13

// ===========================================================================
// LCD CONFIGURATION
// ===========================================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===========================================================================
// HEART-RATE THRESHOLDS
// ===========================================================================

const int LOW_BPM_THRESHOLD  = 60;
const int HIGH_BPM_THRESHOLD = 100;

// ===========================================================================
// SIGNAL PROCESSING PARAMETERS
// ===========================================================================

const int SENSOR_THRESHOLD = 550;
const int RESET_THRESHOLD  = 500;
const int MIN_IBI          = 300;
const int FILTER_SIZE      = 5;

// ===========================================================================
// SYNTHETIC PULSE PARAMETERS
// ===========================================================================

const unsigned long PULSE_WIDTH = 80;

// ===========================================================================
// SYSTEM STATES
// ===========================================================================

enum SystemState {
  STATE_NORMAL,
  STATE_LOW_ALARM,
  STATE_HIGH_ALARM
};

SystemState currentState = STATE_NORMAL;

// ===========================================================================
// SENSOR / FILTER VARIABLES
// ===========================================================================

int rawSensorValue = 0;

int filterBuffer[FILTER_SIZE];
int filterIndex = 0;

int filteredSignal = 0;

// ===========================================================================
// HEARTBEAT VARIABLES
// ===========================================================================

unsigned long lastBeatTime = 0;
unsigned long currentBeatTime = 0;

unsigned long ibi = 800;

int calculatedBPM = 75;

bool peakDetected = false;

// ===========================================================================
// DISPLAY / ALERT TIMERS
// ===========================================================================

unsigned long lastDisplayUpdate = 0;
unsigned long lastBlinkTime = 0;

bool redLedState = false;

// ===========================================================================
// SYNTHETIC PULSE GENERATOR
// ===========================================================================

unsigned long lastSimPulse = 0;

// ===========================================================================
// FUNCTION PROTOTYPES
// ===========================================================================

void initializeFilter();

void readSensorAndFilter();

void generateSyntheticPulse();

void detectHeartbeat();

void calculateBPM();

void evaluateSystemState();

void processAlerts();

void updateDisplay();

void updateSerialMonitor();

// ===========================================================================
// SETUP
// ===========================================================================

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);

  initializeFilter();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // ---------------------------------------------------------
  // Startup screen
  // ---------------------------------------------------------

  lcd.setCursor(0, 0);
  lcd.print("HEART MONITOR");

  lcd.setCursor(0, 1);
  lcd.print("INITIALIZING...");

  delay(2000);

  lcd.clear();

  // ---------------------------------------------------------
  // Serial startup information
  // ---------------------------------------------------------

  Serial.println();
  Serial.println("==============================================");
  Serial.println(" HEARTBEAT MONITOR WITH ALERT SYSTEM");
  Serial.println("==============================================");
  Serial.println("Platform : Arduino UNO / Wokwi");
  Serial.println("Input    : Synthetic PPG via Potentiometer");
  Serial.println("Normal   : 60 - 100 BPM");
  Serial.println("Low Alarm: < 60 BPM");
  Serial.println("High Alarm: > 100 BPM");
  Serial.println("==============================================");
  Serial.println();
}

// ===========================================================================
// MAIN LOOP
// ===========================================================================

void loop() {

  // 1. Read/simulate pulse signal and filter it
  readSensorAndFilter();

  // 2. Detect heartbeat and calculate IBI/BPM
  detectHeartbeat();

  // 3. Determine system state
  evaluateSystemState();

  // 4. Control LEDs and buzzer
  processAlerts();

  // 5. Update LCD and Serial Monitor every 500 ms
  if (millis() - lastDisplayUpdate >= 500) {

    lastDisplayUpdate = millis();

    updateDisplay();
    updateSerialMonitor();
  }
}

// ===========================================================================
// FILTER INITIALIZATION
// ===========================================================================

void initializeFilter() {

  for (int i = 0; i < FILTER_SIZE; i++) {
    filterBuffer[i] = 180;
  }

  filterIndex = 0;
}

// ===========================================================================
// 1. SENSOR READING + SYNTHETIC PULSE GENERATION
// ===========================================================================

void readSensorAndFilter() {

  // Read potentiometer.
  // The potentiometer controls the simulated heart rate.
  int potValue = analogRead(SENSOR_PIN);

  // Convert potentiometer position to pulse period.
  //
  // 1500 ms -> approximately 40 BPM
  // 1000 ms -> approximately 60 BPM
  // 800 ms  -> approximately 75 BPM
  // 600 ms  -> approximately 100 BPM
  // 428 ms  -> approximately 140 BPM

  unsigned long targetPeriod =
      map(potValue, 0, 1023, 1500, 428);

  unsigned long now = millis();

  // Generate a new synthetic pulse at the selected period.
  if (now - lastSimPulse >= targetPeriod) {
    lastSimPulse = now;
  }

  // Generate a short PPG-like pulse peak.
  if (now - lastSimPulse < PULSE_WIDTH) {

    rawSensorValue = 750;

  } else {

    rawSensorValue = 180;
  }

  // ---------------------------------------------------------
  // Moving-average filter
  // ---------------------------------------------------------

  filterBuffer[filterIndex] = rawSensorValue;

  filterIndex++;

  if (filterIndex >= FILTER_SIZE) {
    filterIndex = 0;
  }

  long sum = 0;

  for (int i = 0; i < FILTER_SIZE; i++) {
    sum += filterBuffer[i];
  }

  filteredSignal = sum / FILTER_SIZE;
}

// ===========================================================================
// 2. PEAK DETECTION + IBI MEASUREMENT
// ===========================================================================

void detectHeartbeat() {

  // Detect the beginning of a pulse.
  if (filteredSignal > SENSOR_THRESHOLD && !peakDetected) {

    currentBeatTime = millis();

    // ---------------------------------------------------------
    // First beat establishes the timing baseline.
    // ---------------------------------------------------------

    if (lastBeatTime == 0) {

      lastBeatTime = currentBeatTime;

      peakDetected = true;

      return;
    }

    // ---------------------------------------------------------
    // Calculate time between current and previous beat.
    // ---------------------------------------------------------

    unsigned long timeDelta =
        currentBeatTime - lastBeatTime;

    // ---------------------------------------------------------
    // Accept only physiologically reasonable intervals.
    // ---------------------------------------------------------

    if (timeDelta >= MIN_IBI) {

      ibi = timeDelta;

      lastBeatTime = currentBeatTime;

      peakDetected = true;

      calculateBPM();
    }
  }

  // ---------------------------------------------------------
  // Wait for the signal to return below the reset threshold
  // before allowing another peak to be detected.
  // ---------------------------------------------------------

  if (filteredSignal < RESET_THRESHOLD) {

    peakDetected = false;
  }
}

// ===========================================================================
// 3. BPM CALCULATION
// ===========================================================================

void calculateBPM() {

  if (ibi > 0) {

    // BPM = 60,000 / IBI in milliseconds

    int rawBPM = 60000UL / ibi;

    // Keep the displayed value within a reasonable range.
    calculatedBPM = constrain(rawBPM, 30, 220);
  }
}

// ===========================================================================
// 4. SYSTEM STATE EVALUATION
// ===========================================================================

void evaluateSystemState() {

  if (calculatedBPM < LOW_BPM_THRESHOLD) {

    currentState = STATE_LOW_ALARM;

  }
  else if (calculatedBPM > HIGH_BPM_THRESHOLD) {

    currentState = STATE_HIGH_ALARM;

  }
  else {

    currentState = STATE_NORMAL;
  }
}

// ===========================================================================
// 5. ALERT CONTROL
// ===========================================================================

void processAlerts() {

  switch (currentState) {

    // ---------------------------------------------------------
    // NORMAL STATE
    // ---------------------------------------------------------

    case STATE_NORMAL:

      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);

      noTone(BUZZER_PIN);

      redLedState = false;

      break;

    // ---------------------------------------------------------
    // LOW HEART-RATE ALARM
    // ---------------------------------------------------------

    case STATE_LOW_ALARM:

      digitalWrite(GREEN_LED_PIN, LOW);

      if (millis() - lastBlinkTime >= 250) {

        lastBlinkTime = millis();

        redLedState = !redLedState;

        digitalWrite(RED_LED_PIN, redLedState);
      }

      tone(BUZZER_PIN, 400);

      break;

    // ---------------------------------------------------------
    // HIGH HEART-RATE ALARM
    // ---------------------------------------------------------

    case STATE_HIGH_ALARM:

      digitalWrite(GREEN_LED_PIN, LOW);

      if (millis() - lastBlinkTime >= 120) {

        lastBlinkTime = millis();

        redLedState = !redLedState;

        digitalWrite(RED_LED_PIN, redLedState);
      }

      tone(BUZZER_PIN, 1200);

      break;
  }
}

// ===========================================================================
// 6. LCD DISPLAY
// ===========================================================================

void updateDisplay() {

  // ---------------------------------------------------------
  // Line 1: Heart rate
  // ---------------------------------------------------------

  lcd.setCursor(0, 0);

  lcd.print("HR: ");

  lcd.print(calculatedBPM);

  lcd.print(" BPM     ");

  // ---------------------------------------------------------
  // Line 2: Current state
  // ---------------------------------------------------------

  lcd.setCursor(0, 1);

  switch (currentState) {

    case STATE_NORMAL:

      lcd.print("STATUS: NORMAL  ");

      break;

    case STATE_LOW_ALARM:

      lcd.print("STATUS: LOW     ");

      break;

    case STATE_HIGH_ALARM:

      lcd.print("STATUS: HIGH    ");

      break;
  }
}

// ===========================================================================
// 7. SERIAL MONITOR TELEMETRY
// ===========================================================================

void updateSerialMonitor() {

  Serial.print("RAW_ADC:");
  Serial.print(rawSensorValue);

  Serial.print(" | FILTERED:");
  Serial.print(filteredSignal);

  Serial.print(" | IBI_MS:");
  Serial.print(ibi);

  Serial.print(" | BPM:");
  Serial.print(calculatedBPM);

  Serial.print(" | STATE:");

  switch (currentState) {

    case STATE_NORMAL:

      Serial.println("NORMAL");

      break;

    case STATE_LOW_ALARM:

      Serial.println("ALARM_LOW");

      break;

    case STATE_HIGH_ALARM:

      Serial.println("ALARM_HIGH");

      break;
  }
}