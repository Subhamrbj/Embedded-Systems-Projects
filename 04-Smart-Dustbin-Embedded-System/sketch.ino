#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
// ============================================================================
// HARDWARE PIN DEFINITIONS
// ============================================================================

// HC-SR04 #1 - Hand/Object Detection
#define HAND_TRIG_PIN     7
#define HAND_ECHO_PIN     6

// HC-SR04 #2 - Waste Level Detection
#define BIN_TRIG_PIN      5
#define BIN_ECHO_PIN      4

// Servo Motor
#define SERVO_PIN         9

// Buzzer
#define BUZZER_PIN        10

// Status LEDs
#define GREEN_LED_PIN     12
#define RED_LED_PIN       13


// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================

// Hand detection threshold
const int HAND_DETECTION_DIST_CM = 15;

// Dustbin calibration
const int BIN_EMPTY_DISTANCE_CM = 30;
const int BIN_FULL_DISTANCE_CM  = 3;

// Full-bin alert threshold
const int FULL_THRESHOLD_PERCENT = 85;

// Lid open holding time
const unsigned long LID_HOLD_TIME = 3000;

// LCD update interval
const unsigned long DISPLAY_UPDATE_INTERVAL = 300;


// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

Servo lidServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);


// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

int handDistanceCm = 100;
int wasteDistanceCm = BIN_EMPTY_DISTANCE_CM;
int fillPercentage = 0;

bool isLidOpen = false;

unsigned long lidOpenStartTime = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastBlinkTime = 0;

bool redLedState = false;


// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

long measureDistanceCm(int trigPin, int echoPin);

void processAutoLid();

void calculateBinLevel();

void updateAlertsAndDisplay();


// ============================================================================
// SETUP
// ============================================================================

void setup() {

  Serial.begin(115200);

  delay(100);


  // --------------------------------------------------------------------------
  // PIN CONFIGURATION
  // --------------------------------------------------------------------------

  pinMode(HAND_TRIG_PIN, OUTPUT);
  pinMode(HAND_ECHO_PIN, INPUT);

  pinMode(BIN_TRIG_PIN, OUTPUT);
  pinMode(BIN_ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);


  // --------------------------------------------------------------------------
  // INITIAL OUTPUT STATES
  // --------------------------------------------------------------------------

  digitalWrite(BUZZER_PIN, LOW);

  digitalWrite(GREEN_LED_PIN, HIGH);

  digitalWrite(RED_LED_PIN, LOW);


  // --------------------------------------------------------------------------
  // SERVO INITIALIZATION
  // --------------------------------------------------------------------------

  lidServo.attach(SERVO_PIN);

  // 0 degrees = lid closed
  lidServo.write(0);


  // --------------------------------------------------------------------------
  // LCD INITIALIZATION
  // --------------------------------------------------------------------------

  lcd.init();

  lcd.backlight();

  lcd.clear();


  // --------------------------------------------------------------------------
  // STARTUP SCREEN
  // --------------------------------------------------------------------------

  lcd.setCursor(0, 0);
  lcd.print("SMART DUSTBIN");

  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");

  delay(2000);

  lcd.clear();


  // --------------------------------------------------------------------------
  // SERIAL STARTUP MESSAGE
  // --------------------------------------------------------------------------

  Serial.println();
  Serial.println("==============================================");
  Serial.println(" SMART DUSTBIN CONTROLLER ONLINE");
  Serial.println("==============================================");
  Serial.println("Hand Detection : HC-SR04");
  Serial.println("Bin Level      : HC-SR04");
  Serial.println("Lid Control    : Servo");
  Serial.println("Display        : I2C LCD");
  Serial.println("Alert          : LED + Buzzer");
  Serial.println("==============================================");
  Serial.println();
}


// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {

  // --------------------------------------------------------------------------
  // READ HAND DETECTION SENSOR
  // --------------------------------------------------------------------------

  handDistanceCm =
      measureDistanceCm(HAND_TRIG_PIN, HAND_ECHO_PIN);


  // --------------------------------------------------------------------------
  // READ BIN LEVEL SENSOR
  // --------------------------------------------------------------------------

  wasteDistanceCm =
      measureDistanceCm(BIN_TRIG_PIN, BIN_ECHO_PIN);


  // --------------------------------------------------------------------------
  // PROCESS AUTOMATIC LID
  // --------------------------------------------------------------------------

  processAutoLid();


  // --------------------------------------------------------------------------
  // CALCULATE BIN FILL LEVEL
  // --------------------------------------------------------------------------

  calculateBinLevel();


  // --------------------------------------------------------------------------
  // UPDATE DISPLAY AND ALERTS
  // --------------------------------------------------------------------------

  if (millis() - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {

    lastDisplayUpdate = millis();

    updateAlertsAndDisplay();
  }
}


// ============================================================================
// ULTRASONIC DISTANCE MEASUREMENT
// ============================================================================

long measureDistanceCm(int trigPin, int echoPin) {

  // Make sure trigger starts LOW
  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);


  // Send 10 microsecond trigger pulse
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  // Read echo pulse
  // Timeout = 30 ms
  long durationUs = pulseIn(
    echoPin,
    HIGH,
    30000
  );


  // Invalid / timeout reading
  if (durationUs == 0) {

    return 400;
  }


  // Distance calculation
  // Speed of sound ≈ 0.0343 cm/us
  long distanceCm =
      (durationUs * 0.0343) / 2;


  return distanceCm;
}


// ============================================================================
// AUTOMATIC LID CONTROL
// ============================================================================

void processAutoLid() {

  // --------------------------------------------------------------------------
  // HAND DETECTED
  // --------------------------------------------------------------------------

  if (handDistanceCm <= HAND_DETECTION_DIST_CM) {

    // Open lid only if currently closed
    if (!isLidOpen) {

      lidServo.write(90);

      isLidOpen = true;

      Serial.println(
        "[ACTION] Hand detected -> Lid OPEN"
      );
    }


    // Keep resetting timer while hand remains nearby
    lidOpenStartTime = millis();
  }


  // --------------------------------------------------------------------------
  // AUTOMATIC LID CLOSING
  // --------------------------------------------------------------------------

  if (
    isLidOpen &&
    (millis() - lidOpenStartTime >= LID_HOLD_TIME)
  ) {

    lidServo.write(0);

    isLidOpen = false;

    Serial.println(
      "[ACTION] Timeout -> Lid CLOSED"
    );
  }
}


// ============================================================================
// BIN LEVEL CALCULATION
// ============================================================================

void calculateBinLevel() {

  // --------------------------------------------------------------------------
  // HANDLE INVALID SENSOR READING
  // --------------------------------------------------------------------------

  if (
    wasteDistanceCm < BIN_FULL_DISTANCE_CM ||
    wasteDistanceCm > BIN_EMPTY_DISTANCE_CM
  ) {

    // Keep value within safe physical range
    wasteDistanceCm =
        constrain(
          wasteDistanceCm,
          BIN_FULL_DISTANCE_CM,
          BIN_EMPTY_DISTANCE_CM
        );
  }


  // --------------------------------------------------------------------------
  // CALCULATE FILL PERCENTAGE
  // --------------------------------------------------------------------------

  int distanceRange =
      BIN_EMPTY_DISTANCE_CM -
      BIN_FULL_DISTANCE_CM;


  int filledDistance =
      BIN_EMPTY_DISTANCE_CM -
      wasteDistanceCm;


  fillPercentage =
      map(
        filledDistance,
        0,
        distanceRange,
        0,
        100
      );


  // Safety constraint
  fillPercentage =
      constrain(
        fillPercentage,
        0,
        100
      );
}


// ============================================================================
// ALERT + LCD + SERIAL TELEMETRY
// ============================================================================

void updateAlertsAndDisplay() {

  bool isFull =
      (fillPercentage >= FULL_THRESHOLD_PERCENT);


  // ==========================================================================
  // NORMAL BIN
  // ==========================================================================

  if (!isFull) {

    digitalWrite(
      GREEN_LED_PIN,
      HIGH
    );

    digitalWrite(
      RED_LED_PIN,
      LOW
    );

    noTone(BUZZER_PIN);

    redLedState = false;
  }


  // ==========================================================================
  // FULL BIN ALERT
  // ==========================================================================

  else {

    digitalWrite(
      GREEN_LED_PIN,
      LOW
    );


    // Non-blocking red LED flashing
    if (
      millis() - lastBlinkTime >= 200
    ) {

      lastBlinkTime = millis();

      redLedState =
          !redLedState;

      digitalWrite(
        RED_LED_PIN,
        redLedState
      );
    }


    // Buzzer alert
    tone(
      BUZZER_PIN,
      1200
    );
  }


  // ==========================================================================
  // LCD DISPLAY
  // ==========================================================================

  lcd.clear();


  // Line 1
  // Example:
  // LID:OPEN   75%
  // LID:CLOSE  20%

  lcd.setCursor(0, 0);

  lcd.print("LID:");

  if (isLidOpen) {
    lcd.print("OPEN ");
  }
  else {
    lcd.print("CLOSE");
  }


  lcd.setCursor(10, 0);

  if (fillPercentage < 100) {
    lcd.print(" ");
  }

  if (fillPercentage < 10) {
    lcd.print(" ");
  }

  lcd.print(fillPercentage);

  lcd.print("%");


  // --------------------------------------------------------------------------
  // LINE 2 - SYSTEM STATUS
  // --------------------------------------------------------------------------

  lcd.setCursor(0, 1);

  if (isFull) {

    lcd.print("BIN: FULL ALERT");
  }
  else {

    lcd.print("BIN: NORMAL");
  }


  // ==========================================================================
  // SERIAL MONITOR TELEMETRY
  // ==========================================================================

  Serial.print("HAND_DIST:");

  Serial.print(handDistanceCm);

  Serial.print("cm | WASTE_DIST:");

  Serial.print(wasteDistanceCm);

  Serial.print("cm | FILL:");

  Serial.print(fillPercentage);

  Serial.print("% | LID:");

  if (isLidOpen) {
    Serial.print("OPEN");
  }
  else {
    Serial.print("CLOSED");
  }

  Serial.print(" | STATUS:");

  if (isFull) {

    Serial.println("ALERT_BIN_FULL");
  }
  else {

    Serial.println("NORMAL");
  }
}