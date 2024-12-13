#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Création d'une instance du shield moteur
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

#define CPT_LEFT 3
#define CPT_RIGHT 2

#define CPT_US_TRIG_PIN 10
#define CPT_US_ECHO_PIN 11

#define SPEED 50

// Variable pour la distance
float distance = 0.0;
// Distance seuil en cm
const float seuil = 20.0;
// Variable booléenne
bool estProche = true;

void setup() {
  // Démarrer la communication série
  Serial.begin(9600);
  while (!Serial); // Attendre que la connexion série soit établie

  // Log que le programme a démarré
  Serial.println("Programme démarré!");

  pinMode(CPT_LEFT, INPUT);
  pinMode(CPT_RIGHT, INPUT);

  pinMode(CPT_US_TRIG_PIN, OUTPUT);
  pinMode(CPT_US_ECHO_PIN, INPUT);

  // Initialisation du shield moteur
  AFMS.begin();
  delay(5000);  // Attendre l'initialisation du shield moteur

  // Log que le shield moteur est initialisé
  Serial.println("Shield moteur initialisé.");
}

void loop() {
  // Lire les valeurs des capteurs
  bool left = digitalRead(CPT_LEFT);  // Détection du noir
  bool right = digitalRead(CPT_RIGHT); // Détection du noir

  // Log des valeurs détectées par les capteurs
  Serial.print("CPT_LEFT: ");
  Serial.print(left);
  Serial.print("  CPT_RIGHT: ");
  Serial.println(right);

   // Génération de l'impulsion sur le Trig
  digitalWrite(CPT_US_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(CPT_US_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(CPT_US_TRIG_PIN, LOW);

  // Lecture de l'impulsion sur l'Echo
  long duration = pulseIn(CPT_US_ECHO_PIN, HIGH);

  // Calcul de la distance en cm
  distance = (duration * 0.034) / 2;

  // Mise à jour de la variable booléenne
  if (distance > seuil) {
    estProche = false;
  } else {
    estProche = true;
  }

  if(!estProche){
    Serial.println("NO OBSTACLE");
    if (left && right) {
      Serial.println("FORWARD");
      myMotor1->setSpeed(SPEED);
      myMotor2->setSpeed(SPEED);
      myMotor1->run(FORWARD);
      myMotor2->run(FORWARD);
    } else if (left && !right) {
      Serial.println("LEFT");
      myMotor1->setSpeed(SPEED);
      myMotor1->run(RELEASE);
      myMotor2->run(FORWARD);
    } else if (!left && right) {
      Serial.println("RIGHT");
      myMotor2->setSpeed(SPEED);
      myMotor1->run(FORWARD);
      myMotor2->run(RELEASE);
    } else {
      Serial.println("STOP");
      myMotor1->run(RELEASE);
      myMotor2->run(RELEASE);
    }
  } else {
    Serial.println("OBSTACLE");
    Serial.println("STOP");
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
  }

 // delay(1000);

}
