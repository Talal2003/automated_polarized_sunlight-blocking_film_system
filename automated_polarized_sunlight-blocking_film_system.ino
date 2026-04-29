#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper stepperName(stepsPerRevolution, 52, 51, 53, 50);

const int lightPin = A0;

// Polarizer segment angles (degrees)
const int segments[] = {45, 90, 135, 180, 225, 270, 315, 360};
const int numSegments = sizeof(segments) / sizeof(segments[0]);

// Track stepper position
int currentSegmentIndex = 0;

// Energy-saving mode
bool autoEnergySaving = true;
unsigned long lastAdjustTime = 0;
const unsigned long adjustInterval = 5000;

int degreesToSteps(int degrees) {
  return (long(degrees) * stepsPerRevolution) / 360;
}

// Map light value (0–1023) to segment index
int lightToSegment(int lightValue) {
  int index = map(lightValue, 0, 700, 0, numSegments - 1);
  if (index < 0) index = 0;
  if (index >= numSegments) index = numSegments - 1;
  return index;
}

void setup() {
  stepperName.setSpeed(5);
  Serial.begin(9600);
}

void loop() {
  int lightValue = analogRead(lightPin);
  Serial.print("lightValue: ");
  Serial.println(lightValue);

  int targetSegmentIndex = lightToSegment(lightValue);

  // Only adjust if state changed or auto-saving disabled
  if ((targetSegmentIndex != currentSegmentIndex) &&
      (!autoEnergySaving || millis() - lastAdjustTime > adjustInterval)) {

    // Calculate rotation needed from current segment to target
    int rotationDegrees = segments[targetSegmentIndex] - segments[currentSegmentIndex];

    // Convert to steps and move stepper
    stepperName.step(degreesToSteps(rotationDegrees));
    Serial.print("Rotated ");
    Serial.print(rotationDegrees);
    Serial.println(" degrees.");

    currentSegmentIndex = targetSegmentIndex;
    lastAdjustTime = millis();
  }

  // Check light every second
  delay(1000);
}
