#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 52, 51, 53, 50);

const int lightPin = A0;

const int segments[] = { 0, 45, 90, 135, 180, 225, 270, 315, 360 };
const int numSegments = sizeof(segments) / sizeof(segments[0]);

int currentSegment = 0;

int degreesToSteps(int degrees) {
  return (long)degrees * stepsPerRevolution / 360;
}

void setup() {
  stepper.setSpeed(5);
  Serial.begin(9600);
}

void loop() {
  int lightValue = analogRead(lightPin);

  // Print the light sensor value
  Serial.print("Light value: ");
  Serial.println(lightValue);

  int targetSegment = map(lightValue, 0, 1000, 0, numSegments - 1);

  if (targetSegment != currentSegment) {
    int rotation = segments[targetSegment] - segments[currentSegment];

    // Move the stepper
    stepper.step(degreesToSteps(rotation));

    // Print the rotation
    Serial.print("Rotated: ");
    Serial.print(rotation);
    Serial.println(" degrees");

    currentSegment = targetSegment;
  }

  delay(1000);
}
