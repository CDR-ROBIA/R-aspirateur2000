 #ifndef CONFIG_H
#define CONFIG_H

// parametre du robot
#define COEFR2L 0.96 //0.96
#define BIASR2L 1

#define D_BETWEEN_WHEEL 222 //mm 
#define CENTER2WHEEL 111 //mm
#define DIAM_WHEEL  70.0 //mm
#define nbPPR 341.2 // le nombre de pulsation des encodeurs par tour apres le réducteur 34:1

#define NBMOTOR 2

// ---------------PIN----------------

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


// Collision and Distance
// Left Sensor
#define L_Collision A0
#define L_Distance A1

// Right Sensor
#define R_Collision A2
#define R_Distance A3


#endif 