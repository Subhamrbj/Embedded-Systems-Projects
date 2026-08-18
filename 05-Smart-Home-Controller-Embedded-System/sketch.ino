#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

/* =========================================================
   PIN CONFIGURATION
   ========================================================= */

#define DHT_PIN             4
#define DHT_TYPE            DHT22

#define PIR_PIN             27
#define LDR_PIN             34

#define ROOM_LIGHT_LED      18
#define FAN_LED             19
#define BUZZER_PIN          23
#define SECURITY_LED        25
#define SAFE_LED            26

#define MANUAL_LIGHT_BTN    32
#define MANUAL_FAN_BTN      33
#define SECURITY_BTN        14

#define OLED_SDA            21
#define OLED_SCL            22

/* =========================================================
   OLED CONFIGURATION
   ========================================================= */

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define OLED_RESET          -1
#define OLED_ADDRESS        0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

/* =========================================================
   DHT SENSOR
   ========================================================= */

DHT dht(DHT_PIN, DHT_TYPE);

/* =========================================================
   SYSTEM VARIABLES
   ========================================================= */

float temperature = 0.0;
float humidity = 0.0;

int ldrValue = 0;
int motion = 0;

/* Automatic states */

bool lightState = false;
bool fanState = false;

/* Manual override */

bool manualLight = false;
bool manualFan = false;

/* Security */

bool securityArmed = false;
bool securityAlarm = false;

/* =========================================================
   THRESHOLDS
   ========================================================= */

const int DARK_THRESHOLD = 1800;

/*
   Temperature hysteresis:

   Fan ON  >= 28°C
   Fan OFF <= 27°C
*/

const float FAN_ON_TEMP = 28.0;
const float FAN_OFF_TEMP = 27.0;

/*
   Turn light OFF after no motion for this duration
*/

const unsigned long LIGHT_TIMEOUT = 10000;

unsigned long lastMotionTime = 0;

/* =========================================================
   BUTTON VARIABLES
   ========================================================= */

bool lastLightButton = HIGH;
bool lastFanButton = HIGH;
bool lastSecurityButton = HIGH;

/* =========================================================
   SENSOR TIMER
   ========================================================= */

unsigned long lastSensorRead = 0;

const unsigned long SENSOR_INTERVAL = 2000;

/* =========================================================
   SERIAL TIMER
   ========================================================= */

unsigned long lastDisplayUpdate = 0;

const unsigned long DISPLAY_INTERVAL = 2000;

/* =========================================================
   SETUP
   ========================================================= */

void setup() {

  Serial.begin(115200);

  delay(1500);

  /* ---------- GPIO ---------- */

  pinMode(PIR_PIN, INPUT);

  pinMode(LDR_PIN, INPUT);

  pinMode(ROOM_LIGHT_LED, OUTPUT);
  pinMode(FAN_LED, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(SECURITY_LED, OUTPUT);
  pinMode(SAFE_LED, OUTPUT);

  pinMode(
    MANUAL_LIGHT_BTN,
    INPUT_PULLUP
  );

  pinMode(
    MANUAL_FAN_BTN,
    INPUT_PULLUP
  );

  pinMode(
    SECURITY_BTN,
    INPUT_PULLUP
  );

  /* ---------- Initial states ---------- */

  digitalWrite(ROOM_LIGHT_LED, LOW);
  digitalWrite(FAN_LED, LOW);

  digitalWrite(BUZZER_PIN, LOW);

  digitalWrite(SECURITY_LED, LOW);
  digitalWrite(SAFE_LED, HIGH);

  /* ---------- DHT ---------- */

  dht.begin();

  /* ---------- OLED ---------- */

  Wire.begin(
    OLED_SDA,
    OLED_SCL
  );

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    Serial.println(
      "OLED initialization failed!"
    );
  }

  display.clearDisplay();

  display.setTextColor(
    SSD1306_WHITE
  );

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println(
    "SMART HOME"
  );

  display.println(
    "CONTROLLER"
  );

  display.println();

  display.println(
    "ESP32 SYSTEM"
  );

  display.println(
    "ONLINE"
  );

  display.display();

  /* ---------- Startup Serial ---------- */

  Serial.println();
  Serial.println();
  Serial.println(
    "============================================================"
  );
  Serial.println(
    "                 SMART HOME CONTROLLER"
  );
  Serial.println(
    "                    ESP32 + WOKWI"
  );
  Serial.println(
    "============================================================"
  );

  Serial.println(
    "ESP32 SYSTEM ONLINE"
  );

  Serial.println(
    "Serial communication working!"
  );

  Serial.println(
    "Sensors and actuators initialized."
  );

  Serial.println(
    "============================================================"
  );

  delay(2000);
}

/* =========================================================
   MAIN LOOP
   ========================================================= */

void loop() {

  readSensors();

  handleButtons();

  automaticLightControl();

  automaticFanControl();

  securitySystem();

  updateOutputs();

  updateOLED();

  updateSerialDashboard();

  delay(50);
}

/* =========================================================
   READ SENSORS
   ========================================================= */

void readSensors() {

  if (
    millis() -
    lastSensorRead <
    SENSOR_INTERVAL
  ) {

    return;
  }

  lastSensorRead = millis();

  /* ---------- DHT22 ---------- */

  float newHumidity = dht.readHumidity();
  float newTemperature = dht.readTemperature();

  if (!isnan(newTemperature)) {

    temperature = newTemperature;
  }

  if (!isnan(newHumidity)) {

    humidity = newHumidity;
  }

  /* ---------- LDR ---------- */

  ldrValue = analogRead(
    LDR_PIN
  );

  /* ---------- PIR ---------- */

  motion = digitalRead(
    PIR_PIN
  );

  if (motion == HIGH) {

    lastMotionTime = millis();
  }
}

/* =========================================================
   BUTTON HANDLING
   ========================================================= */

void handleButtons() {

  /* ---------- LIGHT BUTTON ---------- */

  bool currentLightButton =
    digitalRead(
      MANUAL_LIGHT_BTN
    );

  if (
    lastLightButton == HIGH &&
    currentLightButton == LOW
  ) {

    manualLight = !manualLight;

    Serial.println();

    if (manualLight) {

      Serial.println(
        "MANUAL LIGHT OVERRIDE: ON"
      );

    } else {

      Serial.println(
        "MANUAL LIGHT OVERRIDE: OFF"
      );
    }

    delay(150);
  }

  lastLightButton =
    currentLightButton;


  /* ---------- FAN BUTTON ---------- */

  bool currentFanButton =
    digitalRead(
      MANUAL_FAN_BTN
    );

  if (
    lastFanButton == HIGH &&
    currentFanButton == LOW
  ) {

    manualFan = !manualFan;

    Serial.println();

    if (manualFan) {

      Serial.println(
        "MANUAL FAN OVERRIDE: ON"
      );

    } else {

      Serial.println(
        "MANUAL FAN OVERRIDE: OFF"
      );
    }

    delay(150);
  }

  lastFanButton =
    currentFanButton;


  /* ---------- SECURITY BUTTON ---------- */

  bool currentSecurityButton =
    digitalRead(
      SECURITY_BTN
    );

  if (
    lastSecurityButton == HIGH &&
    currentSecurityButton == LOW
  ) {

    securityArmed =
      !securityArmed;

    if (!securityArmed) {

      securityAlarm = false;

      digitalWrite(
        BUZZER_PIN,
        LOW
      );

    }

    Serial.println();

    if (securityArmed) {

      Serial.println(
        "SECURITY SYSTEM: ARMED"
      );

    } else {

      Serial.println(
        "SECURITY SYSTEM: DISARMED"
      );
    }

    delay(150);
  }

  lastSecurityButton =
    currentSecurityButton;
}

/* =========================================================
   AUTOMATIC LIGHT CONTROL
   ========================================================= */

void automaticLightControl() {

  if (manualLight) {

    lightState = true;

    return;
  }

  /*
     If the room is dark and motion is detected,
     turn the light ON.
  */

  if (
    ldrValue < DARK_THRESHOLD &&
    motion == HIGH
  ) {

    lightState = true;

    lastMotionTime = millis();

    return;
  }

  /*
     If there is no motion for a while,
     turn the light OFF.
  */

  if (
    millis() -
    lastMotionTime >
    LIGHT_TIMEOUT
  ) {

    lightState = false;
  }
}

/* =========================================================
   AUTOMATIC FAN CONTROL
   ========================================================= */

void automaticFanControl() {

  if (manualFan) {

    fanState = true;

    return;
  }

  /*
     Hysteresis prevents rapid ON/OFF switching.
  */

  if (
    temperature >= FAN_ON_TEMP
  ) {

    fanState = true;
  }

  else if (
    temperature <= FAN_OFF_TEMP
  ) {

    fanState = false;
  }
}

/* =========================================================
   SECURITY SYSTEM
   ========================================================= */

void securitySystem() {

  if (
    securityArmed &&
    motion == HIGH
  ) {

    securityAlarm = true;
  }

  if (!securityArmed) {

    securityAlarm = false;
  }
}

/* =========================================================
   UPDATE OUTPUTS
   ========================================================= */

void updateOutputs() {

  /* ---------- Room Light ---------- */

  digitalWrite(
    ROOM_LIGHT_LED,
    lightState
      ? HIGH
      : LOW
  );

  /* ---------- Fan ---------- */

  digitalWrite(
    FAN_LED,
    fanState
      ? HIGH
      : LOW
  );

  /* ---------- Security ---------- */

  if (securityAlarm) {

    digitalWrite(
      SECURITY_LED,
      HIGH
    );

    digitalWrite(
      SAFE_LED,
      LOW
    );

    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

  }

  else {

    digitalWrite(
      SECURITY_LED,
      LOW
    );

    digitalWrite(
      SAFE_LED,
      HIGH
    );

    digitalWrite(
      BUZZER_PIN,
      LOW
    );
  }
}

/* =========================================================
   OLED DISPLAY
   ========================================================= */

void updateOLED() {

  if (
    millis() -
    lastDisplayUpdate <
    DISPLAY_INTERVAL
  ) {

    return;
  }

  lastDisplayUpdate = millis();

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println(
    "SMART HOME"
  );

  display.println(
    "CONTROLLER"
  );

  display.println(
    "----------------"
  );

  display.print(
    "T:"
  );

  display.print(
    temperature,
    1
  );

  display.print(
    "C H:"
  );

  display.print(
    humidity,
    0
  );

  display.println(
    "%"
  );

  display.print(
    "LDR:"
  );

  display.println(
    ldrValue
  );

  display.print(
    "Motion:"
  );

  if (motion) {

    display.println(
      "YES"
    );

  } else {

    display.println(
      "NO"
    );
  }

  display.print(
    "Light:"
  );

  display.println(
    lightState
      ? "ON"
      : "OFF"
  );

  display.print(
    "Fan:"
  );

  display.println(
    fanState
      ? "ON"
      : "OFF"
  );

  display.display();
}

/* =========================================================
   SERIAL MONITOR DASHBOARD
   ========================================================= */

void updateSerialDashboard() {

  static unsigned long lastPrint = 0;

  if (
    millis() -
    lastPrint <
    DISPLAY_INTERVAL
  ) {

    return;
  }

  lastPrint = millis();

  Serial.println();

  Serial.println(
    "============================================================"
  );

  Serial.println(
    "                 SMART HOME CONTROLLER"
  );

  Serial.println(
    "============================================================"
  );

  Serial.println();

  Serial.println(
    "SYSTEM STATUS"
  );

  Serial.println(
    "------------------------------------------------------------"
  );

  Serial.print(
    "Temperature : "
  );

  Serial.print(
    temperature,
    1
  );

  Serial.println(
    " C"
  );

  Serial.print(
    "Humidity    : "
  );

  Serial.print(
    humidity,
    1
  );

  Serial.println(
    " %"
  );

  Serial.print(
    "LDR Value   : "
  );

  Serial.println(
    ldrValue
  );

  Serial.print(
    "Room Light  : "
  );

  if (
    ldrValue < DARK_THRESHOLD
  ) {

    Serial.println(
      "DARK"
    );

  } else {

    Serial.println(
      "BRIGHT"
    );
  }

  Serial.print(
    "Motion      : "
  );

  if (motion) {

    Serial.println(
      "DETECTED"
    );

  } else {

    Serial.println(
      "NO MOTION"
    );
  }

  Serial.println();

  Serial.println(
    "ACTUATORS"
  );

  Serial.println(
    "------------------------------------------------------------"
  );

  Serial.print(
    "Room Light  : "
  );

  Serial.println(
    lightState
      ? "ON"
      : "OFF"
  );

  Serial.print(
    "Fan         : "
  );

  Serial.println(
    fanState
      ? "ON"
      : "OFF"
  );

  Serial.print(
    "Light Mode  : "
  );

  Serial.println(
    manualLight
      ? "MANUAL"
      : "AUTO"
  );

  Serial.print(
    "Fan Mode    : "
  );

  Serial.println(
    manualFan
      ? "MANUAL"
      : "AUTO"
  );

  Serial.println();

  Serial.println(
    "SECURITY"
  );

  Serial.println(
    "------------------------------------------------------------"
  );

  Serial.print(
    "Security    : "
  );

  Serial.println(
    securityArmed
      ? "ARMED"
      : "DISARMED"
  );

  Serial.print(
    "Alarm       : "
  );

  Serial.println(
    securityAlarm
      ? "TRIGGERED"
      : "SAFE"
  );

  Serial.println();

  Serial.println(
    "============================================================"
  );
}