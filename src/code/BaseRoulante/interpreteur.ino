#include <Arduino.h>

// ================== ENCODEURS ==================
#define ENC_LEFT_A 2
#define ENC_LEFT_B 6

#define ENC_RIGHT_A 3
#define ENC_RIGHT_B 7

// Compteurs d'impulsions
volatile long ticksLeft = 0;
volatile long ticksRight = 0;

// Interruptions
void compteurLeft() {
    ticksLeft++;
}

void compteurRight() {
    ticksRight++;
}

void setupEncodeurs() {
    ticksLeft = 0;
    ticksRight = 0;

    pinMode(ENC_LEFT_A, INPUT_PULLUP);
    pinMode(ENC_RIGHT_A, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENC_LEFT_A), compteurLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_A), compteurRight, RISING);
}

float ticksRoueGauche() {
    return ticksLeft;
}

float ticksRoueDroite() {
    return ticksRight;
}


#define TOL_ENC 10
#define MIN_DT_BLOCK 200  // temps minimum entre deux checks (ms)

// Sauvegarde des anciennes valeurs
static long ancienTicksLeft = 0;
static long ancienTicksRight = 0;
static unsigned long lastCheck = 0;
static bool initi = false;

void resetEncoders() {
    noInterrupts();
    ticksLeft = 0;
    ticksRight = 0;
    interrupts();

    ancienTicksLeft = 0;
    ancienTicksRight = 0;
    lastCheck = 0;
    initi = false;
}

bool rouesBloquees() {

    unsigned long now = millis();

    if (now - lastCheck < MIN_DT_BLOCK) {
        return false;
    }

    lastCheck = now;

    long actuelLeft, actuelRight;

    noInterrupts();
    actuelLeft = ticksLeft;
    actuelRight = ticksRight;
    interrupts();

    // 🔥 IGNORE PREMIER PASSAGE
    if (!initi) {
        ancienTicksLeft = actuelLeft;
        ancienTicksRight = actuelRight;
        initi = true;
        return false;
    }

    long diffLeft = abs(actuelLeft - ancienTicksLeft);
    long diffRight = abs(actuelRight - ancienTicksRight);

    ancienTicksLeft = actuelLeft;
    ancienTicksRight = actuelRight;

    bool bloque = (diffLeft < TOL_ENC || diffRight < TOL_ENC);

    return bloque;
}


// ================== ULTRASON ==================

#include <Ultrasonic.h>
#define US_left_trigger 13
#define US_left_echo 12

#define US_right_trigger 11
#define US_right_echo 10

Ultrasonic ultrasonicLeft(US_left_trigger, US_left_echo);
Ultrasonic ultrasonicRight(US_right_trigger, US_right_echo);

int distanceGauche() {
    int distance = ultrasonicLeft.read();

    // Sécurité si lecture invalide
    if (distance <= 0 || distance > 400) {
        return -1;
    }

    return distance;
}

int distanceDroite() {
    int distance = ultrasonicRight.read();

    // Sécurité si lecture invalide
    if (distance <= 0 || distance > 400) {
        return -1;
    }

    return distance;
}

bool chercheObstacle(int d) {

    // Cas spécial : désactivé
    if (d == -1) {
        return false;
    }

    // Lecture capteur gauche
    int gauche = distanceGauche();

    // Petit délai pour éviter les interférences
    delay(50);

    // Lecture capteur droit
    int droite = distanceDroite();

    // Vérifie si un obstacle est détecté
    if ((gauche != -1 && gauche < d) ||
        (droite != -1 && droite < d)) {
        return true;
    }

    return false;
}


// ================== MOTEURS ==================
#define ML_FWD 4
#define ML_BWD 5
#define MR_FWD 9
#define MR_BWD 8

#define vitesseMotor 200
#define offsetGauche 0  // peut être positif ou négatif


void setupMotor() {
    pinMode(ML_FWD, OUTPUT);
    pinMode(ML_BWD, OUTPUT);
    pinMode(MR_FWD, OUTPUT);
    pinMode(MR_BWD, OUTPUT);
    moteursStop(); // sécurité au démarrage
}

void moteursStop() {
    analogWrite(ML_FWD, 0);
    analogWrite(ML_BWD, 0);
    analogWrite(MR_FWD, 0);
    analogWrite(MR_BWD, 0);
}
void moteursAvance() {

    int vLeft = vitesseMotor + offsetGauche;
    int vRight = vitesseMotor;

    vLeft = constrain(vLeft, 0, 255);
    vRight = constrain(vRight, 0, 255);

    analogWrite(ML_FWD, vLeft);
    analogWrite(ML_BWD, 0);

    analogWrite(MR_FWD, vRight);
    analogWrite(MR_BWD, 0);
}

void moteursRecule() {

    int vLeft = vitesseMotor + offsetGauche;
    int vRight = vitesseMotor;

    vLeft = constrain(vLeft, 0, 255);
    vRight = constrain(vRight, 0, 255);

    analogWrite(ML_FWD, 0);
    analogWrite(ML_BWD, vLeft);

    analogWrite(MR_FWD, 0);
    analogWrite(MR_BWD, vRight);
}

void moteursTourneGauche() {

    int vLeft = vitesseMotor + offsetGauche;
    int vRight = vitesseMotor;

    vLeft = constrain(vLeft, 0, 255);
    vRight = constrain(vRight, 0, 255);

    analogWrite(ML_FWD, 0);
    analogWrite(ML_BWD, vLeft);

    analogWrite(MR_FWD, vRight);
    analogWrite(MR_BWD, 0);
}

void moteursTourneDroite() {

    int vLeft = vitesseMotor + offsetGauche;
    int vRight = vitesseMotor;

    vLeft = constrain(vLeft, 0, 255);
    vRight = constrain(vRight, 0, 255);

    analogWrite(ML_FWD, vLeft);
    analogWrite(ML_BWD, 0);

    analogWrite(MR_FWD, 0);
    analogWrite(MR_BWD, vRight);
}
// ================== AVANCE ==================

int AVANCE(long dist, int obs) {
resetEncoders();
    moteursAvance();
    while (true) {
        // ================== SECURITE OBSTACLE ==================
        if (chercheObstacle(obs)) {
            moteursStop();
            return 1;
        }
        // ================== SECURITE BLOCAGE ==================
        if (rouesBloquees()) {
            moteursStop();
            return 2;
        }
        // Moyenne des deux roues
        long moyenne = (ticksLeft + ticksRight) / 2;
        // Objectif atteint
        if (moyenne >= dist) {
            moteursStop();
            return 0;
        }
        delay(20);
    }
}

// ================== RECULE ==================

int RECULE(long dist) {
resetEncoders();
    moteursRecule();
    while (true) {
        // ================== BLOCAGE ==================
        if (rouesBloquees()) {
            moteursStop();
            return 2;
        }
        // Moyenne des deux roues
        long moyenne = (ticksLeft + ticksRight) / 2;
        // Objectif atteint
        if (moyenne >= dist) {
            moteursStop();
            return 0;
        }
        delay(20);
    }
}



int TOURNE_GAUCHE(long dist) {
resetEncoders();
    moteursTourneGauche();
    while (true) {

        // blocage roues
        if (rouesBloquees()) {
            moteursStop();
            return 2;
        }

        // moyenne du déplacement (rotation)
        long moyenne = (abs(ticksLeft) + abs(ticksRight)) / 2;

        if (moyenne >= dist) {
            moteursStop();
            return 0;
        }

        delay(20);
    }
}


int TOURNE_DROITE(long dist) {
resetEncoders();
    moteursTourneDroite();

    while (true) {

        // blocage roues
        if (rouesBloquees()) {
            moteursStop();
            return 2;
        }

        // moyenne du déplacement
        long moyenne = (abs(ticksLeft) + abs(ticksRight)) / 2;

        if (moyenne >= dist) {
            moteursStop();
            return 0;
        }

        delay(20);
    }
}

// ================== INTERPRETEUR ==================

void executeCommand(String cmd) {
    

    cmd.trim();

    // ================== ECHO COMMANDE ==================
    Serial.print("CMD: ");
    Serial.println(cmd);

    int code = -1;

    // ================== AVANCE ==================
    if (cmd.startsWith("AVANCE")) {

        int space1 = cmd.indexOf(' ');
        int space2 = cmd.indexOf(' ', space1 + 1);

        long dist = cmd.substring(space1 + 1, space2).toInt();
        int obs = cmd.substring(space2 + 1).toInt();

        code = AVANCE(dist, obs);
    }

    // ================== RECULE ==================
    else if (cmd.startsWith("RECULE")) {

        int space = cmd.indexOf(' ');
        long dist = cmd.substring(space + 1).toInt();

        code = RECULE(dist);
    }

    // ================== TOURNE GAUCHE ==================
    else if (cmd.startsWith("TOURNEGAUCHE")) {

        int space = cmd.indexOf(' ');
        long dist = cmd.substring(space + 1).toInt();

        code = TOURNE_GAUCHE(dist);
    }

    // ================== TOURNE DROITE ==================
    else if (cmd.startsWith("TOURNEDROITE")) {

        int space = cmd.indexOf(' ');
        long dist = cmd.substring(space + 1).toInt();

        code = TOURNE_DROITE(dist);
    }

    else {
        Serial.println("CMD UNKNOWN");
        return;
    }

    // ================== TICKS ==================

    long moy = (abs(ticksLeft) + abs(ticksRight)) / 2;

    // ================== RESULT ==================

    Serial.print(code);
    Serial.print(" ");
    Serial.println(moy);
}

long ticksMoyens() {
    return (ticksLeft + ticksRight) / 2;
}
String inputString = "";
bool stringComplete = false;


// ================== SETUP ==================
void setup() {




  Serial.begin(9600);
    setupEncodeurs();
    setupMotor();

}

void loop() {

    while (Serial.available()) {
        char inChar = (char)Serial.read();

        if (inChar == '\n' || inChar == '\r') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }

    if (stringComplete) {
        executeCommand(inputString);
        inputString = "";
        stringComplete = false;
    }
}