#include <Stepper.h>

// Steps per full revolution
const int stepsPerRevolution = 2048;

// Step amounts
const int steps90  = stepsPerRevolution / 4;  // 90 degrees
const int steps180 = stepsPerRevolution / 2; // 180 degrees

// ULN2003 pins: IN1, IN3, IN2, IN4
Stepper stepper(stepsPerRevolution, 52, 51, 53, 50);

void setup() {
  stepper.setSpeed(10); // RPM
}

void loop() {
  // 90° clockwise
  stepper.step(steps90);
  delay(1000);

  // 180° counter-clockwise
  stepper.step(-steps180);
  delay(1000);

  // 90° clockwise
  stepper.step(steps90);
  delay(1000);
}
