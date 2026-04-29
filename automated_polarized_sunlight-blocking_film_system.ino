#include <Stepper.h>

int buttonPressPin = 38;

bool buttonPressed;

// Set how many steps it takes to make a full revolution. Divide the degrees per step by 360 to get the steps
const int stepsPerRevolution = 2048;

// Use pin 8-11 on the arduino to IN1-IN4 on the stepper board
Stepper stepperName = Stepper(stepsPerRevolution, 52, 51, 53, 50);

void setup() {

  // Set the RPM of the stepper motor
  stepperName.setSpeed(5);

  // Set the pinMode of our button pin
  pinMode(buttonPressPin, INPUT_PULLUP);

  // Set our button press Boolean to a known value
  buttonPressed = true;
}

void loop() {
    // Checking the state of the button pin
    buttonPressed = digitalRead(buttonPressPin);
    // If the button is pressed, step to 90* and reset the button pressing state
    if (buttonPressed == false) {
      stepperName.step(stepsPerRevolution / 4);
      delay(50);
      buttonPressed = true;
    }
    
}