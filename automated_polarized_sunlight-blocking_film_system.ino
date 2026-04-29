#include <Stepper.h>

const int stepsPerRevolution = 2048;

// Stepper motor pins
Stepper stepperName(stepsPerRevolution, 52, 51, 53, 50);

// Button pins
const int button90Pin  = 38;
const int button180Pin = 39;

void setup() {
  stepperName.setSpeed(5);

  pinMode(button90Pin, INPUT_PULLUP);
  pinMode(button180Pin, INPUT_PULLUP);
}

// Convert degrees to steps
int degreesToSteps(int degrees) {
  return (degrees * stepsPerRevolution) / 360;
}

void loop() {

  if (digitalRead(button90Pin) == LOW) {
    stepperName.step(stepsPerRevolution / 4);
    delay(300);
  }

  if (digitalRead(button180Pin) == LOW) {
    stepperName.step(-(stepsPerRevolution / 4));
    delay(300);
  }
}
