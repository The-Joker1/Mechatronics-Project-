#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Création d'une instance du shield moteur
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

// Définitions des broches des encodeurs
#define CPT_LEFT 3   // Broche de l'encodeur du moteur gauche
#define CPT_RIGHT 2  // Broche de l'encodeur du moteur droit

#define SPEED 50  // Vitesse des moteurs (0 à 255)
#define TICKS_PER_CM 2  // 330/(pi*5.8)
#define DISTANCE_CM 15  // Distance cible en cm

// Variables pour compter les ticks
volatile int ticksLeft = 0;
volatile int ticksRight = 0;

// Fonction appelée lors d'une interruption pour compter les ticks du moteur gauche
void countLeft() {
  ticksLeft++;
}

// Fonction appelée lors d'une interruption pour compter les ticks du moteur droit
void countRight() {
  ticksRight++;
}

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);
  Serial.println("Initialisation...");

  // Initialisation du shield moteur
  AFMS.begin();

  // Configuration des broches d'encodeur
  pinMode(CPT_LEFT, INPUT_PULLUP);
  pinMode(CPT_RIGHT, INPUT_PULLUP);

  // Attachement des interruptions pour les encodeurs
  attachInterrupt(digitalPinToInterrupt(CPT_LEFT), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(CPT_RIGHT), countRight, RISING);

  // Démarrage des moteurs
  myMotor1->setSpeed(SPEED);
  myMotor2->setSpeed(SPEED);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
}

void loop() {
  // Calcul du nombre total de ticks requis pour atteindre la distance cible
  int targetTicks = DISTANCE_CM * TICKS_PER_CM;

  // Affiche les ticks actuels (pour le débogage)
  Serial.print("Ticks Left: ");
  Serial.print(ticksLeft);
  Serial.print(" | Ticks Right: ");
  Serial.println(ticksRight);

  // Vérifie si les deux moteurs ont atteint la distance cible
  if (ticksLeft >= targetTicks && ticksRight >= targetTicks) {
    stopMotors(); // Arrête les moteurs
    Serial.println("Distance atteinte !");
    while (1);    // Arrêt du programme
  }
}

// Fonction pour arrêter les moteurs
void stopMotors() {
  myMotor1->run(RELEASE);
  myMotor2->run(RELEASE);
}
