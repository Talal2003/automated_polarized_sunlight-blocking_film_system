#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper stepperName(stepsPerRevolution, 52, 51, 53, 50);

const int lightPin = A0;

// Light ranges
const int noLightThreshold  = 300;
const int highLightThreshold = 700;

// Track last state so it doesn't repeat
int lastState = 0;

int degreesToSteps(int degrees) {
  return (degrees * stepsPerRevolution) / 360;
}

void setup() {
  stepperName.setSpeed(5);
  Serial.begin(9600);
}

void loop() {
  int lightValue = analogRead(lightPin);
  Serial.println(lightValue);

  // BELOW 300, do nothing
  if (lightValue < noLightThreshold) {
    lastState = 0;   // reset state so it can trigger again later
  }

  // BETWEEN 300 AND 700, 90 degrees
  else if (lightValue >= noLightThreshold && lightValue <= highLightThreshold) {
    if (lastState != 1) {
      stepperName.step(stepsPerRevolution / 4);
      lastState = 1;
    }
  }

  // ABOVE 700, 180 degrees
  else if (lightValue > highLightThreshold) {
    if (lastState != 2) {
      stepperName.step(stepsPerRevolution / 2);
      lastState = 2;
    }
  }

  delay(100);
}
