#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <PID_v1.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

#define CPT_LEFT 3
#define CPT_RIGHT 2

#define SPEED 100

// PID parameters
double setpointLeft = 0;
double setpointRight = 0;
double inputLeft, outputLeft, inputRight, outputRight;
double Kp = 2, Ki = 5, Kd = 1;
PID pidLeft(&inputLeft, &outputLeft, &setpointLeft, Kp, Ki, Kd, DIRECT);
PID pidRight(&inputRight, &outputRight, &setpointRight, Kp, Ki, Kd, DIRECT);

volatile long encoderPosLeft = 0;
volatile long encoderPosRight = 0;

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

  // Initialize PID controllers
  pidLeft.SetMode(AUTOMATIC);
  pidRight.SetMode(AUTOMATIC);

  // Attach interrupts for encoders
  attachInterrupt(digitalPinToInterrupt(CPT_LEFT), updateEncoderLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CPT_RIGHT), updateEncoderRight, CHANGE);
}

void loop() {
  bool left = digitalRead(CPT_LEFT);
  bool right = digitalRead(CPT_RIGHT);

  // Log what the sensors detect
  Serial.print("Left sensor: ");
  Serial.print(left ? "HIGH" : "LOW");
  Serial.print("  Right sensor: ");
  Serial.println(right ? "HIGH" : "LOW");

  inputLeft = encoderPosLeft;
  inputRight = encoderPosRight;

  pidLeft.Compute();
  pidRight.Compute();

  int speedLeft = map(outputLeft, 0, 255, 0, SPEED);
  int speedRight = map(outputRight, 0, 255, 0, SPEED);

  if (!left && !right) {
    Serial.println("Both sensors are LOW. Moving forward.");
    myMotor1->setSpeed(speedLeft);
    myMotor2->setSpeed(speedRight);
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
  } else if (!left && right) {
    Serial.println("Left sensor is LOW, Right sensor is HIGH. Turning left.");
    myMotor1->setSpeed(speedLeft);
    myMotor1->run(RELEASE);
    myMotor2->run(FORWARD);
  } else if (left && !right) {
    Serial.println("Left sensor is HIGH, Right sensor is LOW. Turning right.");
    myMotor2->setSpeed(speedRight);
    myMotor1->run(FORWARD);
    myMotor2->run(RELEASE);
  } else {
    Serial.println("Both sensors are HIGH. Stopping motors.");
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
  }

  delay(1000);  // Small delay to make the serial output readable
}

void updateEncoderLeft() {
  encoderPosLeft += (digitalRead(CPT_LEFT) == HIGH) ? 1 : -1;
}

void updateEncoderRight() {
  encoderPosRight += (digitalRead(CPT_RIGHT) == HIGH) ? 1 : -1;
}

"""explications : entrée --> encodeurs, sorties --> vitesse des moteurs """