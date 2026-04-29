#include <Wire.h>
#include <BH1750.h>
#include <Stepper.h>

// Stepper setup
const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);

// BH1750 setup
BH1750 lightMeter;

// Segments
const int segments[] = { 0, 45, 90, 135, 180, 225, 270, 315, 360 };
const int numSegments = sizeof(segments) / sizeof(segments[0]);
int currentSegment = 0;

int degreesToSteps(int degrees) {
  return (long)degrees * stepsPerRevolution / 360;
}

void setup() {
  Serial.begin(9600);

  // Stepper speed
  stepper.setSpeed(5);

  // Start BH1750
  Wire.begin();
  if (lightMeter.begin()) {
    Serial.println("BH1750 initialized.");
  } else {
    Serial.println("Error initializing BH1750.");
  }
}

void loop() {
  float lux = lightMeter.readLightLevel(); // Read in lux
  Serial.print("Light level: ");
  Serial.println(lux);

  int targetSegment = map(lux, 0, 1000, 0, numSegments - 1);

  targetSegment = constrain(targetSegment, 0, numSegments - 1);

  if (targetSegment != currentSegment) {
    int rotation = segments[targetSegment] - segments[currentSegment];
    stepper.step(degreesToSteps(rotation));

    Serial.print("Rotated: ");
    Serial.print(rotation);
    Serial.println(" degrees");

    currentSegment = targetSegment;
  }

  delay(1000);
}
