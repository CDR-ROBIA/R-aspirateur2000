#include <CytronMotorDriver.h>

#include "raspi_config.h"
#include "raspi_capteur.h"
#include "raspi_motion.h"

static unsigned long lastMs = 0;
const unsigned long intervalMs = 20;


const int enca[] = {L_EncoderA, R_EncoderA};
const int encb[] = {L_EncoderB, R_EncoderB};
const int inA[] = {L_MotorA, R_MotorA};
const int inB[] = {L_MotorB, R_MotorB};

volatile int posi[] = {0,0};


float wheel_perimeter = PI*DIAM_WHEEL;
int speed = 60;
float actionfaite[3] = {0,0,0};
float move[5] = {0,0,1,1,200};
float default_move[5] = {0,0,1,1,200};


CytronMD motor_Left( PWM_PWM, inA[0], inB[0]);
CytronMD motor_Right(PWM_PWM, inA[1], inB[1]);

template <int j>
void readEncoder(){
  int b = digitalRead(encb[j]);
  if(b > 0){
    posi[j]--;
  }
  else{
    posi[j]++;
  }
}

int dist2encodeurvalue(float distmm){
  int nb_pulsation;
  nb_pulsation = (distmm/wheel_perimeter)*nbPPR;
  return nb_pulsation;
}


float angle2encodervalue(float angle){
  float P_arc;
  int nbPulseAngle;
  // convertir un angle en distance
  P_arc= (PI*D_BETWEEN_WHEEL*angle)/360;
  nbPulseAngle = dist2encodeurvalue(P_arc);
  return nbPulseAngle;
}

int encodeurvalue2dist(int pulse){
  float dist;
  dist = (pulse*wheel_perimeter)/nbPPR;
  return dist;
}
float encodervalue2angle(int pulse){
  float angle;
  float dist = encodeurvalue2dist(pulse);
  angle = (dist*360)/(PI*D_BETWEEN_WHEEL);
  return angle;
}


float getspeed(long pos, float dtime){
  float Covers = pos/dtime;
  float rpm = (Covers/nbPPR)*60.0;
  //float mps = (rpm*PI*(DIAM_WHEEL/1000))/60;
  return Covers;
}


// Get distance from pololu sensor
int getDistance(int sensorPin) {
  int16_t t ;
  int flag = 0;
  while (flag == 0) {
    t = pulseIn(sensorPin, HIGH);
    if (t == 0) {
    } else if (t > 1850) {
      return 500;      
    } else {
      int16_t d = (t - 1000) * 3 / 4;
      if (d < 0) { d = 0; }
      return d;
    }
  }
}

// get Status from switch
int getStop(int sensorPin) {
  int val = analogRead(sensorPin);
  if (val <= 512) { return 0; } 
  else            { return 1; }
}

bool iscollision_front(float dist_secu){
  int Lldist = getDistance(L_Distance);
  int Rldist = getDistance(R_Distance);
  if (Lldist < dist_secu || Rldist < dist_secu){
    return true;
  }
  else {
    return false;
  }
}

bool iscollision_LR(){
  if (getStop(L_Collision) || getStop(R_Collision)){
    return true;
  }
  else {
    return false;
  }
}

void reset_values(){
  for (int j=0; j<5;++j){
    move[j]=default_move[j];
  }
}

float get_move() {
  int c=0;
  char buffer[32];  // tableau pour stocker la réception
  if (Serial.available() > 0){
    int n = Serial.readBytesUntil('\"', buffer, sizeof(buffer)-1);
    buffer[n] = '\0';
    //Serial.print(buffer);  
    // Pointeur pour stocker chaque morceau
    char *token = strtok(buffer, ",");

    while (token != NULL) {
    // convertir le morceau en entier
      if ( c<5){
          //Serial.println(value);
        move[c]=atof(token);
      }
      ++c;

      token = strtok(NULL, ","); // passer au suivant
    }
  }
}

void send_action_done(float action_done[3]) {
  for (int i=0; i<3;i++){
    Serial.print(action_done[i]);
    Serial.print(",");
  }
  Serial.println();
}

struct move_info {
  float completion_action; // dans une transaltion = la distance parcourus, dans une rotations l'angle fait 
  int type_erreur; 
};

struct move_info linear_move(float distance) {
  if (distance > 0.0) {
    dirL=1;
    dirR=1;
  }
  else {
    dirL=-1;
    dirR=-1;
  }
  // on transforme la distance en nombre de pulsation
  target = dist2encodeurvalue(distance);
  // on set la vitesse 
  speed_left = dirL*speed;
  speed_right = dirR*(COEFR2L*speed + BIASR2L);

  while (true){
    // recuperation des comptes de pulsation
    noInterrupts();
    cL = posi[0];
    cR = posi[1];
    interrupts();

    // test des condition qui meme a l'arret de la base roulante 
    if (fabs(cR) >= target) { // action entierement faite
      stoptype=0;
      break;  // on sort de la boucle
    }
    else if (front_col && iscollision_front(dsec)){ // distance de securite a l'avant
      stoptype=1;
      break;
    }
    else if (coter_col && iscollision_LR()){ // les sswitch sur les coté du robot
      stoptype=2;
      break;
    }
    else {
      motor_Left.setSpeed(speed_left); motor_Right.setSpeed(speed_right);
    }
  }
  // on sauvegarde la distance/angle parcouru avant l'interuption
  motor_Left.setSpeed(0); motor_Right.setSpeed(0);
  return {pulse2value(cL), stoptype};
}
    

void setup() {
  Serial.begin(9600);

  pinMode(L_Collision , INPUT);
  pinMode(L_Distance  , INPUT);

  pinMode(R_Collision , INPUT);
  pinMode(R_Distance  , INPUT);


  for (int i=0; i<NBMOTOR; i++){
    pinMode(enca[i] , INPUT);
    pinMode(encb[i] , INPUT);
    pinMode(inA[i]  , OUTPUT);
    pinMode(inB[i]  , OUTPUT);
  /*pinMode(L_EncoderA  , INPUT);
  pinMode(L_EncoderB  , INPUT);
  pinMode(L_MotorB    , OUTPUT);
  pinMode(L_MotorA    , OUTPUT);

  pinMode(R_EncoderA  , INPUT);
  pinMode(R_EncoderB  , INPUT);
  pinMode(R_MotorB    , OUTPUT);
  pinMode(R_MotorA    , OUTPUT);*/
  }
    // Initialize Encoder
  attachInterrupt(digitalPinToInterrupt(L_EncoderA), readEncoder<0>, RISING);
  attachInterrupt(digitalPinToInterrupt(R_EncoderA), readEncoder<1>, RISING);

}

void loop(){  
  reset_values();
  get_move();
  dsec = move[4];
  front_col=move[2];
  coter_col=move[3];
  move_info linear_result = linear_move(100);
  actionfaite[1] = linear_result.completion_action;
  actionfaite[2] = linear_result.type_erreur;
  
  send_action_done(actionfaite);

}