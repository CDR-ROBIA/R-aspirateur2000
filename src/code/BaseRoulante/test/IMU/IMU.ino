#include <Arduino_LSM6DSOX.h>

#define ALPHA 0.98 // filtre complémentaire : 0.98 gyro, 0.02 accel

float pitch = 0.0;
float roll = 0.0;
unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU non détecté !");
    while (1);
  }

  Serial.println("IMU prêt");
  lastTime = millis();
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz); // en degrés/sec

    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0; // secondes
    lastTime = currentTime;

    // Accéléromètre → angles (estimation lente mais stable)
    float pitchAcc = atan2(ay, sqrt(ax * ax + az * az)) * 180 / PI;
    float rollAcc  = atan2(-ax, az) * 180 / PI;

    // Gyroscope → vitesse angulaire
    pitch = ALPHA * (pitch + gy * dt) + (1 - ALPHA) * pitchAcc;
    roll  = ALPHA * (roll + gx * dt) + (1 - ALPHA) * rollAcc;

    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print("  Roll: ");
    Serial.println(roll);
  }

  delay(10);
}
