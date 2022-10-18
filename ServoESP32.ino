#include <ESP32Servo.h>
#include <ESP32PWM.h>

// Servo objects for each motor || Objetos Servo para cada motor
Servo servo1;

//Published values for SG90 - MG90 motors || Valores publicados para los motores SG90 - MG90
int minUs = 500;
int maxUs = 2400;

//GPIO pins for servo motors || Pines GPIO de la ESP32 para los servo motores
int servo1Pin = 15;

int pos = 0; // Position in degrees || posicion en grados para el motor

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Serial.begin(115200);
  servo1.setPeriodHertz(50);

  servo1.attach(servo1Pin, minUs, maxUs);
}

void loop() {
  for (pos = 0; pos <= 90; pos += 1) { // sweep from 0 degrees to 90 degrees || gira desde 0 a 90 grados
    // in steps of 1 degree
    servo1.write(pos);
    delay(1);
  }
  delay(3000);
  for (pos = 90; pos >= 0; pos -= 1) { // sweep from 90 degrees to 0 degrees || gira desde 0 a 90 grados
    servo1.write(pos);
    delay(1);
  }
}
