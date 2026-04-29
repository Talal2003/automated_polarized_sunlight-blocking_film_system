#include <AccelStepper.h>

#define MotorInterfaceType AccelStepper::HALF4WIRE

// Pins IN1, IN3, IN2, IN4
AccelStepper stepper(MotorInterfaceType, 52, 51, 53, 50);

void setup() {
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);
}

void loop() {
  // 90° clockwise
  stepper.move(2048);
  stepper.runToPosition();


  // 180° counter-clockwise
  stepper.move(-2048);
  stepper.runToPosition();


  // 90° clockwise (back to start)
  stepper.move(2048);
  stepper.runToPosition();

}
