#include <ESP32Servo.h>
#include <ESP32PWM.h>

// Servo objects for each motor || Objetos Servo para cada motor
Servo servo1, servo2, servo3, servo4;

int startBtn = 12;

bool avoidTimer = false;
bool system_ready = false;
bool motor_running = false;

//Published values for SG90 - MG90 motors || Valores publicados para los motores SG90 - MG90
int minUs = 500;
int maxUs = 2400;
int readyTime;

//GPIO pins for servo motors || Pines GPIO de la ESP32 para los servo motores
int servo1Pin = 15;
int servo2Pin = 16;
int servo3Pin = 14;
int servo4Pin = 4;

//GPIO pins for potentiometers || Pines GPIO para los potenciometros
int potPin1 = 34;
int potPin2 = 35;
int potPin3 = 32;
int potPin4 = 33;

//GPIO pin for the mixer motor || Pin GPIO para la mexcladora
int mixer = 21;

int pos = 0; // Position in degrees || posicion en grados para el motor
int val1, val2, val3, val4;  // Variables to read the values from the analog pins (potentiometers connected)
int time1, time2, time3, time4; //Variables to set as a timer for the motors to move at a direction

int ADC_Max = 4096;     // This is the default ADC max value on the ESP32 (12 bit ADC width);

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Serial.begin(115200);
  servo1.setPeriodHertz(50);

  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  servo4.attach(servo4Pin, minUs, maxUs);
  
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(potPin3, INPUT);
  pinMode(potPin4, INPUT);
  
  pinMode(startBtn, INPUT);
  pinMode(mixer, OUTPUT);

  digitalWrite(mixer, LOW);
}

void loop() {
  val1 = analogRead(potPin1);
  val1 = map(val1, 0, ADC_Max, 0, 100);

  val2 = analogRead(potPin2);
  val2 = map(val2, 0, ADC_Max, 0, 100);

  val3 = analogRead(potPin3);
  val3 = map(val3, 0, ADC_Max, 0, 100);

  val4 = analogRead(potPin4);
  val4 = map(val4, 0, ADC_Max, 0, 100);
  
  Serial.println(val1);

  if(digitalRead(startBtn) == HIGH) {
    time1 = (val1 * 20) / 100;
    time2 = (val2 * 20) / 100;
    time3 = (val3 * 20) / 100;
    time4 = (val4 * 20) / 100;
   
    system_ready = true;
    avoidTimer = true;
  }

  if(system_ready == true) {
    servoControl(time1, time2, time3, time4);
  }

  if(readyTime >= 0 && motor_running == true) {
    digitalWrite(mixer, HIGH);
    readyTime--;

    delay(1000);
  }
  else {
    digitalWrite(mixer, LOW);
  }
}

void servoControl(int t1, int t2, int t3, int t4) {
 if(t1 > 0) {
    t1--;
    Serial.println("active");
    servo1.write(90);
    delay(100);

    motor_running = false;
  }
   else {
    servo1.write(0);
    motor_running = true;
  }

  if(t2 > 0) {
    t2--;
    servo2.write(90);
    delay(100);

    motor_running = false;
  }
  else {
    servo2.write(0);
    motor_running = true;
  }

  if(t3 > 0) {
    t3--;
    servo3.write(90);
    delay(100);

    motor_running = false;
  }
  else {
    servo3.write(0);
    motor_running = true;
  }

  if(t4 > 0) {
    t4--;
    servo4.write(90);
    delay(100);

    motor_running = false;
  }
  else {
    servo4.write(0);
    motor_running = true;
  } 

  time1 = t1;
  time2 = t2;
  time3 = t3;
  time4 = t4;

  if(avoidTimer == true && time1 <= 0 && time2 <= 0 && time3 <= 0 && time4 <= 0) {
    readyTime = 10;
    avoidTimer = false;
  }
}
