#include <Wire.h>
#include <BH1750.h>
#include <Stepper.h>

// Stepper setup
const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);

// BH1750 setup
BH1750 lightMeter1(0x23);  // first sensor
BH1750 lightMeter2(0x5C);  // second sensor

// Segments
const int segments[] = { 0, 45, 90, 135, 180, 225, 270, 315, 360 };
const int numSegments = sizeof(segments) / sizeof(segments[0]);
int currentSegment = 0;

int degreesToSteps(int degrees) {
  return (long)degrees * stepsPerRevolution / 360;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  stepper.setSpeed(5);

  if (!lightMeter1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23)) {
    Serial.println("Error initializing BH1750 #1");
  }

  if (!lightMeter2.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C)) {
    Serial.println("Error initializing BH1750 #2");
  }
}

void loop() {
  float lux1 = lightMeter1.readLightLevel();
  float lux2 = lightMeter2.readLightLevel();

  Serial.print("Light sensor 1: ");
  Serial.println(lux1);
  Serial.print("Light sensor 2: ");
  Serial.println(lux2);

  float lux = (lux1 + lux2) / 2;

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
