#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

#define CPT_LEFT 3
#define CPT_RIGHT 2

#define SPEED 100

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial); // Wait for the serial connection to be established

  // Log that the program has started
  Serial.println("Program started!");

  pinMode(CPT_LEFT, INPUT);
  pinMode(CPT_RIGHT, INPUT);

  AFMS.begin();
  delay(5000);  // Wait for motor shield initialization

  // Log motor shield initialization
  Serial.println("Motor shield initialized.");
}

void loop() {
  bool left = digitalRead(CPT_LEFT);
  bool right = digitalRead(CPT_RIGHT);

  // Log what the sensors detect
  Serial.print("Left sensor: ");
  Serial.print(left ? "HIGH" : "LOW");
  Serial.print("  Right sensor: ");
  Serial.println(right ? "HIGH" : "LOW");

  if (!left && !right) {
    Serial.println("Both sensors are LOW. Moving forward.");
    myMotor1->setSpeed(SPEED);
    myMotor2->setSpeed(SPEED);
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
  } else if (!left && right) {
    Serial.println("Left sensor is LOW, Right sensor is HIGH. Turning left.");
    myMotor1->setSpeed(SPEED);
    myMotor1->run(RELEASE);
    myMotor2->run(FORWARD);
  } else if (left && !right) {
    Serial.println("Left sensor is HIGH, Right sensor is LOW. Turning right.");
    myMotor2->setSpeed(SPEED);
    myMotor1->run(FORWARD);
    myMotor2->run(RELEASE);
  } else {
    Serial.println("Both sensors are HIGH. Stopping motors.");
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
  }

  delay(1000);  // Small delay to make the serial output readable
}