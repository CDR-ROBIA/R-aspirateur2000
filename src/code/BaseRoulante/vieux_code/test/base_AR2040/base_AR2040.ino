#include <CytronMotorDriver.h>

// Pin motors Left
#define L_EncoderA 6
#define L_EncoderB 7
#define L_MotorB 8
#define L_MotorA 9

// Pin motors Right
#define R_EncoderA 2
#define R_EncoderB 3
#define R_MotorB 4
#define R_MotorA 5

CytronMD motor_Left( PWM_PWM, L_MotorA, L_MotorB);
// vitesse du moteur droit = vitesse moteur gauche -9
// 128 = 655
//  x  = 610
CytronMD motor_Right(PWM_PWM, R_MotorA, R_MotorB);

long countL_nb = 0;
long countR_nb = 0;

int il = 0;
int ir = 0;

// parametre du robot
#define COEFR2L 0.96
#define BIASR2L 0

void countL() { countL_nb += 1; }
void countR() { countR_nb += 1; }

void setup() {
  Serial.begin(9600);

  pinMode(L_EncoderA  , INPUT);
  pinMode(L_EncoderB  , INPUT);
  pinMode(L_MotorB    , OUTPUT);
  pinMode(L_MotorA    , OUTPUT);

  pinMode(R_EncoderA  , INPUT);
  pinMode(R_EncoderB  , INPUT);
  pinMode(R_MotorB    , OUTPUT);
  pinMode(R_MotorA    , OUTPUT);

    // Initialize Encoder
  attachInterrupt(digitalPinToInterrupt(L_EncoderA), countL, RISING);
  attachInterrupt(digitalPinToInterrupt(R_EncoderA), countR, RISING);
}

void loop(){
  delay(10000);
  countL_nb = 0; countR_nb = 0;
  for (float i=25; i<128; i++){
    il = i;
    ir = (COEFR2L * i) + BIASR2L;
    motor_Left.setSpeed(il); motor_Right.setSpeed(ir);
    delay(10);
    Serial.print(i);
    Serial.print(",");
    Serial.print(countL_nb);
    Serial.print(",");
    Serial.print(countR_nb);
    Serial.print("\n");
  }
  for (float i=128; i>25; i--){
    il = i;
    ir = (COEFR2L * i) + BIASR2L;
    motor_Left.setSpeed(il); motor_Right.setSpeed(ir);
    delay(10);
    Serial.print(i);
    Serial.print(",");
    Serial.print(countL_nb);
    Serial.print(",");
    Serial.print(countR_nb);
    Serial.print("\n");
  }
  
  motor_Left.setSpeed(0); motor_Right.setSpeed(0);
  

  
}