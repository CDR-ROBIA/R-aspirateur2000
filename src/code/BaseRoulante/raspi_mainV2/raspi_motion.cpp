/*#include <CytronMotorDriver.h>
#include <Arduino.h>

#include "raspi_config.h"
#include "raspi_motion.h"

int speed_left;
int speed_right;
float countL_nb;
float countR_nb;

float dist_between_wheel = 22.2;
float diameter_wheel = 70.0;

float wheel_perimeter = 2*PI*diameter_wheel; 


CytronMD motor_Left( PWM_PWM, L_MotorA, L_MotorB);
CytronMD motor_Right(PWM_PWM, R_MotorA, R_MotorB);

void countL() { countL_nb += 1; }
void countR() { countR_nb += 1; }


int forward(int speed){
  countL_nb=0;
  countR_nb=0;
  speed_left = speed;
  speed_right = (COEFR2L * speed) -2 ;
  motor_Left.setSpeed(speed_left); motor_Right.setSpeed(speed_right);
  for (int i; i< 1000; i++){
  //Serial.print(N);
  //Serial.print(",");
  //Serial.print(speed);
  //Serial.print(",");
  //Serial.print(speed_left);
  //Serial.print(",");
  //Serial.print(speed_right);
  //Serial.print(",");
    Serial.println(countL_nb);
    //Serial.print(",");
    //Serial.print(countR_nb);
  //Serial.print("\n");
    delay(1);
  }
  return 0;

  int target = 600;

  // PID constants
  float kp = 0.1;
  float kd = 0.025;
  float ki = 0.0;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  int pos = 0; 
  pos = posi;

  // error
  int e = pos - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > 128 ){
    pwr = 128;
  }
  if( pwr < 25 ){
    pwr = 25;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  int speed = dir*pwr;
  motor_Left.setSpeed(speed);


  // store previous error
  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.print(" ");
  Serial.print(speed);
  Serial.println();
}*/


/*

void rotate(float angle, int speed=50){
  int dirL=1;
  int dirR=1;

  if (angle >0){
    dirL=-1;
    dirR=1;
  }
  else if (angle < 0){
    dirL=1;
    dirR=-1;
  }
  int target = dirL*angle2encodervalue(angle);
  speed_left = speed*dirL;
  speed_right = (COEFR2L * speed)*dirR;
  motor_Left.setSpeed(speed_left); motor_Right.setSpeed(speed_right);
  while (dirL*posi < target){
    int e = dirL*target - dirL*posi;
    Serial.print(e);
    Serial.print("||");
    Serial.print(posi);
    Serial.println();
  }
  delay(3000);
  motor_Left.setSpeed(0); motor_Right.setSpeed(0);
  }

  
float wheel_perimeter = PI*DIAM_WHEEL; 



void ForBackWard(float dist, int speed=50){
  int dir;
  if (dist > 0){
    dir = 1;
  }
  else{
    dir=-1;
  }
  int target = dir*dist2encodeurvalue(dist);
  Serial.println(target);
  delay(3000);
  speed_left = speed*dir;
  speed_right = (COEFR2L * speed)*dir;
  motor_Left.setSpeed(speed_left); motor_Right.setSpeed(speed_right);
  while (dir*posi < target){
    int e = dir*target - dir*posi;
    Serial.print(e);
    Serial.print("||");
    Serial.print(posi);
    Serial.println();
  }
  motor_Left.setSpeed(0); motor_Right.setSpeed(0);
  
//delay(1000);
  /*for (int i=0; i<3000;i++){
    Serial.println();
    Serial.print(posi[0]);
    Serial.print(" || ");
    Serial.print(posi[1]);
    Serial.println();
  }
  unsigned long now = millis();
  if (now - lastMs >= intervalMs) {

    noInterrupts();
    long cA = posi[0]; posi[0] = 0;
    long cB = posi[1]; posi[1] = 0;
    interrupts();

    float dt = (now - lastMs) / 1000.0; // en secondes
    float rpmL = getspeed(cA, dt);
    float rpmR = -1*getspeed(cB, dt);
    Serial.print(lm);
    Serial.print(",");
    Serial.print(rpmL);
    Serial.print(",");
    Serial.print(-1*rpmR);
    Serial.println();


    lastMs = now;
    lm++;
}*/