#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

// TB6600 pins
#define DIR_PIN 2
#define STEP_PIN 3

// Motor settings
const int STEPS_PER_REV = 200;
const int MICROSTEPPING = 16;
int motorRPM = 60;

// Segment settings
const int NUM_SEGMENTS = 8;
const int DEGREES_PER_SEGMENT = 3600;

int currentSegment = 0;

// Light sensors
BH1750 lightMeter1(0x23);
BH1750 lightMeter2(0x5C);

int updateFrequencyMs = 1000;  // Update every 1 second

// Bluetooth ZS-040 TX (RX, TX)
SoftwareSerial BTSerial(10, 11);
bool isAutoMode = true;

// Convert degrees to microsteps
long degreesToSteps(long degrees) {
  return degrees * ((long)STEPS_PER_REV * MICROSTEPPING) / 360;
}

// Step the motor by a signed number of microsteps
void stepMotor(long steps) {
  if (steps == 0)
    return;

  // half-period delay derived from RPM
  long stepsPerSec = ((long)STEPS_PER_REV * MICROSTEPPING * motorRPM) / 60;
  int halfPeriodUs = max(1L, 500000L / stepsPerSec);

  digitalWrite(DIR_PIN, steps > 0 ? HIGH : LOW);
  steps = abs(steps);

  for (long i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(halfPeriodUs);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(halfPeriodUs);
  }
}

// Move to an absolute segment (0..NUM_SEGMENTS). Returns degrees rotated.
int moveToSegment(int targetSegment) {
  targetSegment = constrain(targetSegment, 0, NUM_SEGMENTS);

  if (targetSegment == currentSegment)
    return 0;

  int deltaDeg = (targetSegment - currentSegment) * DEGREES_PER_SEGMENT;
  stepMotor(degreesToSteps(deltaDeg));
  currentSegment = targetSegment;

  // Save to EEPROM (address 0)
  EEPROM.update(0, currentSegment);

  return deltaDeg;
}

// Move by a relative number of segments. Returns degrees rotated.
int moveBySegments(int segmentDelta) {
  return moveToSegment(currentSegment + segmentDelta);
}

// Rotate by arbitrary degrees without updating segment tracking
// (might use for minor adjustments in future)
void moveDegrees(int degrees) {
  stepMotor(degreesToSteps(degrees));
}

int getCurrentSegment() {
  return currentSegment;
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Wire.begin();

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  // Read last saved segment from EEPROM
  currentSegment = EEPROM.read(0);

  if (!lightMeter1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23))
    Serial.println(F("Error initializing BH1750 #1"));

  if (!lightMeter2.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C))
    Serial.println(F("Error initializing BH1750 #2"));
}

// Check Bluetooth input
void checkBluetooth() {
  if (BTSerial.available()) {
    String cmd = BTSerial.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("AUTO")) {
      isAutoMode = true;
      Serial.println("Switched to AUTO mode");
    } else if (cmd.equalsIgnoreCase("MANUAL")) {
      isAutoMode = false;
      Serial.println("Switched to MANUAL mode");
    } else {
      int seg = cmd.toInt();
      if (!isAutoMode && seg >= 0 && seg <= NUM_SEGMENTS) {
        moveToSegment(seg);
        Serial.print("Moved to segment: ");
        Serial.println(seg);
      }
    }
  }
}

void loop() {
  checkBluetooth();

  if (isAutoMode) {
    float lux1 = lightMeter1.readLightLevel();
    float lux2 = lightMeter2.readLightLevel();

    Serial.print(F("Light sensor 1: "));
    Serial.println(lux1);
    Serial.print(F("Light sensor 2: "));
    Serial.println(lux2);

    float lux = (lux1 + lux2) / 2.0f;

    int targetSegment = map((long)lux, 0, 4000, 0, NUM_SEGMENTS);
    targetSegment = constrain(targetSegment, 0, NUM_SEGMENTS);

    int rotated = moveToSegment(targetSegment);

    if (rotated != 0) {
      Serial.print(F("Moved to segment "));
      Serial.print(currentSegment);
      Serial.print(F(" (rotated "));
      Serial.print(rotated);
      Serial.println(F(" deg)"));
    }

    delay(updateFrequencyMs);
  }
}