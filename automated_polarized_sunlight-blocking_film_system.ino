// Include the Arduino stepper library
#include <Stepper.h>

// Steps for one full revolution
const int stepsPerRevolution = 2048;

// Steps for 90 degrees (1/4 turn)
const int stepsPerQuarterTurn = stepsPerRevolution / 4;

// Use pin 50–53 to IN1–IN4
Stepper stepperName(stepsPerRevolution, 52, 53, 51, 50);

void setup() {
  // Set the RPM of the stepper motor
  stepperName.setSpeed(10);
}

void loop() {
  // Rotate 90 degrees
  stepperName.step(stepsPerQuarterTurn);

  // Pause for 1 second
  delay(1000);
}
