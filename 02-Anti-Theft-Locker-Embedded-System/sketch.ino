#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// ============================================================================
// PIN DEFINITIONS
// ============================================================================
#define SERVO_PIN       10
#define BUZZER_PIN      11
#define GREEN_LED_PIN   12
#define RED_LED_PIN     13

// ============================================================================
// KEYPAD CONFIGURATION
// ============================================================================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}

};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);


// ============================================================================
// PERIPHERAL OBJECTS
// ============================================================================

Servo lockerServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);


// ============================================================================
// SECURITY CONFIGURATION
// ============================================================================

// Master password for this educational prototype.
const String CORRECT_PASSWORD = "1234";

// Maximum number of consecutive incorrect attempts.
const byte MAX_FAILED_ATTEMPTS = 3;

// Maximum password length accepted.
const byte MAX_PASSWORD_LENGTH = 8;

// Servo positions.
const byte LOCK_ANGLE = 0;
const byte UNLOCK_ANGLE = 90;

// Time for which locker remains unlocked.
const unsigned long AUTO_LOCK_DELAY = 5000UL;

// Security lockout duration after maximum failed attempts.
const unsigned long LOCKOUT_TIME = 10000UL;


// ============================================================================
// SYSTEM STATES
// ============================================================================

enum SystemState {

  LOCKED_STATE,
  UNLOCKED_STATE,
  LOCKOUT_STATE

};

SystemState currentState = LOCKED_STATE;


// ============================================================================
// SYSTEM VARIABLES
// ============================================================================

String enteredPassword = "";

byte failedAttempts = 0;

unsigned long unlockStartTime = 0;

unsigned long lockoutStartTime = 0;

unsigned long lastAlarmToggle = 0;

bool alarmToneState = false;


// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void initializeHardware();

void showWelcomeScreen();

void showPasswordScreen();

void processKey(char key);

void handleDigit(char key);

void handleSubmit();

void handleClear();

void checkPassword();

void grantAccess();

void lockLocker();

void denyAccess();

void triggerLockout();

void handleLockout();

void resetPasswordInput();

void updatePasswordDisplay();

void setLockedIndicators();

void setUnlockedIndicators();

void setAlarmIndicators();

void stopAlarm();

void beepKey();

void beepAccessGranted();

void beepAccessDenied();

void printSystemStatus();


// ============================================================================
// SETUP
// ============================================================================

void setup() {

  // ------------------------------------------------------------
  // Initialize Serial Monitor
  // ------------------------------------------------------------

  Serial.begin(9600);

  // ------------------------------------------------------------
  // Initialize hardware
  // ------------------------------------------------------------

  initializeHardware();

  // ------------------------------------------------------------
  // Welcome screen
  // ------------------------------------------------------------

  showWelcomeScreen();

  // ------------------------------------------------------------
  // Start in locked state
  // ------------------------------------------------------------

  currentState = LOCKED_STATE;

  setLockedIndicators();

  showPasswordScreen();

  // ------------------------------------------------------------
  // Serial information
  // ------------------------------------------------------------

  Serial.println();
  Serial.println("==============================================");
  Serial.println("       ANTI-THEFT LOCKER SYSTEM");
  Serial.println("==============================================");
  Serial.println("System Status : LOCKED");
  Serial.println("Password      : 1234");
  Serial.println("Max Attempts  : 3");
  Serial.println("Lockout Time  : 10 seconds");
  Serial.println("Auto Lock     : 5 seconds");
  Serial.println("----------------------------------------------");
  Serial.println("# = Submit");
  Serial.println("* = Clear");
  Serial.println("==============================================");
}


// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {

  // ------------------------------------------------------------
  // Handle security lockout
  // ------------------------------------------------------------

  if (currentState == LOCKOUT_STATE) {

    handleLockout();

    return;
  }


  // ------------------------------------------------------------
  // Handle automatic locking
  // ------------------------------------------------------------

  if (currentState == UNLOCKED_STATE) {

    if (millis() - unlockStartTime >= AUTO_LOCK_DELAY) {

      lockLocker();
    }
  }


  // ------------------------------------------------------------
  // Read keypad
  // ------------------------------------------------------------

  char key = keypad.getKey();

  if (key != NO_KEY) {

    processKey(key);
  }
}


// ============================================================================
// HARDWARE INITIALIZATION
// ============================================================================

void initializeHardware() {

  // ------------------------------------------------------------
  // GPIO configuration
  // ------------------------------------------------------------

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(RED_LED_PIN, OUTPUT);


  // ------------------------------------------------------------
  // Initial output states
  // ------------------------------------------------------------

  digitalWrite(BUZZER_PIN, LOW);

  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(RED_LED_PIN, HIGH);


  // ------------------------------------------------------------
  // Servo initialization
  // ------------------------------------------------------------

  lockerServo.attach(SERVO_PIN);

  lockerServo.write(LOCK_ANGLE);


  // ------------------------------------------------------------
  // LCD initialization
  // ------------------------------------------------------------

  lcd.init();

  lcd.backlight();

  lcd.clear();
}


// ============================================================================
// WELCOME SCREEN
// ============================================================================

void showWelcomeScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ANTI-THEFT");

  lcd.setCursor(0, 1);
  lcd.print("LOCKER SYSTEM");

  delay(2000);
}


// ============================================================================
// PASSWORD SCREEN
// ============================================================================

void showPasswordScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ENTER PASSWORD:");

  updatePasswordDisplay();
}


// ============================================================================
// KEYPAD PROCESSOR
// ============================================================================

void processKey(char key) {

  // ------------------------------------------------------------
  // Every valid keypad press produces a short beep.
  // ------------------------------------------------------------

  beepKey();


  // ------------------------------------------------------------
  // Submit password
  // ------------------------------------------------------------

  if (key == '#') {

    handleSubmit();

    return;
  }


  // ------------------------------------------------------------
  // Clear password
  // ------------------------------------------------------------

  if (key == '*') {

    handleClear();

    return;
  }


  // ------------------------------------------------------------
  // Only numerical keys are accepted as password characters.
  // A, B, C and D are ignored.
  // ------------------------------------------------------------

  if (key >= '0' && key <= '9') {

    handleDigit(key);
  }
}


// ============================================================================
// HANDLE NUMERIC PASSWORD DIGIT
// ============================================================================

void handleDigit(char key) {

  // ------------------------------------------------------------
  // Prevent password from exceeding maximum length.
  // ------------------------------------------------------------

  if (enteredPassword.length() >= MAX_PASSWORD_LENGTH) {

    return;
  }


  // ------------------------------------------------------------
  // Add digit to password.
  // ------------------------------------------------------------

  enteredPassword += key;


  // ------------------------------------------------------------
  // Update masked LCD display.
  // ------------------------------------------------------------

  updatePasswordDisplay();
}


// ============================================================================
// HANDLE SUBMIT
// ============================================================================

void handleSubmit() {

  // ------------------------------------------------------------
  // Do not submit an empty password.
  // ------------------------------------------------------------

  if (enteredPassword.length() == 0) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("ENTER PASSWORD");

    lcd.setCursor(0, 1);
    lcd.print("CANNOT BE EMPTY");

    tone(BUZZER_PIN, 300, 300);

    delay(1200);

    showPasswordScreen();

    return;
  }


  // ------------------------------------------------------------
  // Verify password.
  // ------------------------------------------------------------

  checkPassword();
}


// ============================================================================
// HANDLE CLEAR
// ============================================================================

void handleClear() {

  resetPasswordInput();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("INPUT CLEARED");

  tone(BUZZER_PIN, 600, 100);

  delay(500);

  showPasswordScreen();
}


// ============================================================================
// PASSWORD VERIFICATION
// ============================================================================

void checkPassword() {

  lcd.clear();


  // ------------------------------------------------------------
  // CORRECT PASSWORD
  // ------------------------------------------------------------

  if (enteredPassword == CORRECT_PASSWORD) {

    failedAttempts = 0;

    Serial.println();
    Serial.println("[ACCESS GRANTED]");
    Serial.println("Correct password entered.");

    grantAccess();

    return;
  }


  // ------------------------------------------------------------
  // INCORRECT PASSWORD
  // ------------------------------------------------------------

  failedAttempts++;

  Serial.println();
  Serial.println("[ACCESS DENIED]");

  Serial.print("Failed Attempts: ");
  Serial.println(failedAttempts);


  // ------------------------------------------------------------
  // Maximum attempts reached
  // ------------------------------------------------------------

  if (failedAttempts >= MAX_FAILED_ATTEMPTS) {

    triggerLockout();

    return;
  }


  // ------------------------------------------------------------
  // Normal incorrect-password warning
  // ------------------------------------------------------------

  denyAccess();
}


// ============================================================================
// ACCESS GRANTED
// ============================================================================

void grantAccess() {

  currentState = UNLOCKED_STATE;

  unlockStartTime = millis();

  // ------------------------------------------------------------
  // Move servo to unlocked position.
  // ------------------------------------------------------------

  lockerServo.write(UNLOCK_ANGLE);


  // ------------------------------------------------------------
  // Green LED ON / Red LED OFF
  // ------------------------------------------------------------

  setUnlockedIndicators();


  // ------------------------------------------------------------
  // LCD message
  // ------------------------------------------------------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS GRANTED");

  lcd.setCursor(0, 1);
  lcd.print("LOCKER OPEN");


  // ------------------------------------------------------------
  // Success sound
  // ------------------------------------------------------------

  beepAccessGranted();


  // ------------------------------------------------------------
  // Serial information
  // ------------------------------------------------------------

  Serial.println("Locker unlocked.");

  Serial.println("Auto-lock timer started.");

  printSystemStatus();


  // ------------------------------------------------------------
  // Clear password from memory.
  // ------------------------------------------------------------

  resetPasswordInput();
}


// ============================================================================
// LOCKER LOCKING
// ============================================================================

void lockLocker() {

  currentState = LOCKED_STATE;

  // ------------------------------------------------------------
  // Servo returns to locked position.
  // ------------------------------------------------------------

  lockerServo.write(LOCK_ANGLE);


  // ------------------------------------------------------------
  // Indicators
  // ------------------------------------------------------------

  setLockedIndicators();


  // ------------------------------------------------------------
  // LCD
  // ------------------------------------------------------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LOCKER LOCKED");

  lcd.setCursor(0, 1);
  lcd.print("SECURE");


  // ------------------------------------------------------------
  // Serial log
  // ------------------------------------------------------------

  Serial.println();
  Serial.println("[AUTO-LOCK]");
  Serial.println("Locker automatically locked.");


  // ------------------------------------------------------------
  // Clear previous password.
  // ------------------------------------------------------------

  resetPasswordInput();


  // ------------------------------------------------------------
  // Return to password screen after short message.
  // ------------------------------------------------------------

  delay(1000);

  showPasswordScreen();
}


// ============================================================================
// ACCESS DENIED
// ============================================================================

void denyAccess() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS DENIED");


  lcd.setCursor(0, 1);
  lcd.print("TRIES LEFT:");

  lcd.print(MAX_FAILED_ATTEMPTS - failedAttempts);


  // ------------------------------------------------------------
  // Red LED ON temporarily.
  // ------------------------------------------------------------

  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(RED_LED_PIN, HIGH);


  // ------------------------------------------------------------
  // Warning sound.
  // ------------------------------------------------------------

  beepAccessDenied();


  delay(1500);


  // ------------------------------------------------------------
  // Return to password input.
  // ------------------------------------------------------------

  resetPasswordInput();

  showPasswordScreen();


  // ------------------------------------------------------------
  // Restore locked indicators.
  // ------------------------------------------------------------

  setLockedIndicators();
}


// ============================================================================
// SECURITY LOCKOUT
// ============================================================================

void triggerLockout() {

  currentState = LOCKOUT_STATE;

  lockoutStartTime = millis();

  lastAlarmToggle = millis();

  alarmToneState = false;


  // ------------------------------------------------------------
  // Clear entered password.
  // ------------------------------------------------------------

  resetPasswordInput();


  // ------------------------------------------------------------
  // LCD lockout message.
  // ------------------------------------------------------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("TOO MANY");

  lcd.setCursor(0, 1);
  lcd.print("ATTEMPTS!");


  // ------------------------------------------------------------
  // Serial security alert.
  // ------------------------------------------------------------

  Serial.println();
  Serial.println("==============================================");
  Serial.println("!!! SECURITY ALERT !!!");
  Serial.println("!!! TOO MANY ATTEMPTS !!!");
  Serial.println("!!! SYSTEM LOCKED OUT !!!");
  Serial.println("==============================================");


  // ------------------------------------------------------------
  // Ensure servo remains locked.
  // ------------------------------------------------------------

  lockerServo.write(LOCK_ANGLE);
}


// ============================================================================
// HANDLE LOCKOUT
// ============================================================================

void handleLockout() {

  unsigned long elapsed =
      millis() - lockoutStartTime;


  // ------------------------------------------------------------
  // Alternate alarm tone and red LED every 250 ms.
  // ------------------------------------------------------------

  if (millis() - lastAlarmToggle >= 250) {

    lastAlarmToggle = millis();

    alarmToneState = !alarmToneState;


    if (alarmToneState) {

      digitalWrite(RED_LED_PIN, HIGH);

      tone(BUZZER_PIN, 1200);

    }
    else {

      digitalWrite(RED_LED_PIN, LOW);

      tone(BUZZER_PIN, 800);
    }
  }


  // ------------------------------------------------------------
  // Keep green LED OFF during lockout.
  // ------------------------------------------------------------

  digitalWrite(GREEN_LED_PIN, LOW);


  // ------------------------------------------------------------
  // End lockout after configured time.
  // ------------------------------------------------------------

  if (elapsed >= LOCKOUT_TIME) {

    stopAlarm();

    failedAttempts = 0;

    currentState = LOCKED_STATE;

    setLockedIndicators();

    Serial.println();
    Serial.println("[LOCKOUT ENDED]");
    Serial.println("Security system reset.");
    Serial.println("Locker remains LOCKED.");


    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("LOCKOUT ENDED");

    lcd.setCursor(0, 1);
    lcd.print("TRY AGAIN");


    delay(1200);

    showPasswordScreen();
  }
}


// ============================================================================
// RESET PASSWORD INPUT
// ============================================================================

void resetPasswordInput() {

  enteredPassword = "";
}


// ============================================================================
// UPDATE MASKED PASSWORD DISPLAY
// ============================================================================

void updatePasswordDisplay() {

  lcd.setCursor(0, 1);

  lcd.print("PASS:");


  // ------------------------------------------------------------
  // Display one * for every entered character.
  // ------------------------------------------------------------

  for (byte i = 0; i < enteredPassword.length(); i++) {

    lcd.print("*");
  }


  // ------------------------------------------------------------
  // Clear remaining characters on LCD line.
  // ------------------------------------------------------------

  for (
    byte i = enteredPassword.length();
    i < MAX_PASSWORD_LENGTH;
    i++
  ) {

    lcd.print(" ");
  }
}


// ============================================================================
// LOCKED INDICATORS
// ============================================================================

void setLockedIndicators() {

  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(RED_LED_PIN, HIGH);

  noTone(BUZZER_PIN);
}


// ============================================================================
// UNLOCKED INDICATORS
// ============================================================================

void setUnlockedIndicators() {

  digitalWrite(GREEN_LED_PIN, HIGH);

  digitalWrite(RED_LED_PIN, LOW);

  noTone(BUZZER_PIN);
}


// ============================================================================
// ALARM INDICATORS
// ============================================================================

void setAlarmIndicators() {

  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(RED_LED_PIN, HIGH);

  tone(BUZZER_PIN, 1200);
}


// ============================================================================
// STOP ALARM
// ============================================================================

void stopAlarm() {

  noTone(BUZZER_PIN);

  digitalWrite(RED_LED_PIN, LOW);

  digitalWrite(GREEN_LED_PIN, LOW);

  alarmToneState = false;
}


// ============================================================================
// KEYPRESS BEEP
// ============================================================================

void beepKey() {

  tone(BUZZER_PIN, 2000, 50);
}


// ============================================================================
// ACCESS GRANTED SOUND
// ============================================================================

void beepAccessGranted() {

  tone(BUZZER_PIN, 1000, 150);

  delay(180);

  tone(BUZZER_PIN, 1500, 200);
}


// ============================================================================
// ACCESS DENIED SOUND
// ============================================================================

void beepAccessDenied() {

  tone(BUZZER_PIN, 400, 500);
}


// ============================================================================
// SERIAL STATUS
// ============================================================================

void printSystemStatus() {

  Serial.print("State: ");

  switch (currentState) {

    case LOCKED_STATE:

      Serial.println("LOCKED");

      break;

    case UNLOCKED_STATE:

      Serial.println("UNLOCKED");

      break;

    case LOCKOUT_STATE:

      Serial.println("LOCKOUT");

      break;
  }

  Serial.print("Failed Attempts: ");

  Serial.println(failedAttempts);
}