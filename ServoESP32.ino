#include <ESP32Servo.h>

// Servo objects for each motor || Objetos Servo para cada motor
Servo servo1, servo2, servo3, servo4;

// GPIO pins for servo motors and btns
int servoPins[4] = {4, 13, 14, 16};
int startBtns[4] = {34, 35, 32, 33};

//Published values for SG90 - MG90 motors || Valores publicados para los motores SG90 - MG90
int minUs = 500;
int maxUs = 2400;

//GPIO pin for the mixer motor and potentiometer || Pin GPIO para la mexcladora
int mixer = 21;
int potPin = 12;

bool btn_state, btn_state2, btn_state3, btn_state4;
bool servoRead1, servoReady2, servoReady3, servoReady4;

bool lastBtn_state = LOW;
bool lastBtn_state2 = LOW;
bool lastBtn_state3 = LOW;
bool lastBtn_state4 = LOW;

unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;

unsigned long debounceDelay = 90;

int anagRead1, anagRead2, anagRead3, anagRead4;  // Variables to read the values from the analog pins (potentiometers connected)
int val1, val2, val3, val4; //Variables to set as a timer for the motors to move at a direction

bool systemReady;
bool skipMixer = false;

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Serial.begin(115200);
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);
  
  servo1.attach(servoPins[0], minUs, maxUs);
  servo2.attach(servoPins[1], minUs, maxUs);
  servo3.attach(servoPins[2], minUs, maxUs);
  servo4.attach(servoPins[3], minUs, maxUs);

  pinMode(mixerPin, OUTPUT);
  
  for(int i=0; i < 4; i++) {
   pinMode(startBtns[i], INPUT); 
  }
}

void loop() {
  bool btn1 = digitalRead(startBtns[0]);

  if (btn1 != lastBtn_state) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (btn1 != btn_state) {
      btn_state = btn1;
    }

    if (btn_state == HIGH) {
      Serial.println("btn1");

      systemReady = true;
      anagRead1 = analogRead(potPin);
      val1 = map(anagRead1, 0, 4096, 0, 20);
    }
  }

  bool btn2 = digitalRead(startBtns[1]);

  if (btn2 != lastBtn_state2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (btn2 != btn_state2) {
      btn_state2 = btn2;
    }

    if (btn_state2 == HIGH) {
      Serial.println("btn2");

      systemReady = true;
      anagRead2 = analogRead(potPin);
      val2 = map(anagRead2, 0, 4096, 0, 20);
    }
  }

  bool btn3 = digitalRead(startBtns[2]);

  if (btn3 != lastBtn_state3) {
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime3) > debounceDelay) {
    if (btn3 != btn_state3) {
      btn_state3 = btn3;
    }

    if (btn_state3 == HIGH) {
      Serial.println("btn3");

      systemReady = true;
      anagRead3 = analogRead(potPin);
      val3 = map(anagRead3, 0, 4096, 0, 20);
    }
  }

  bool btn4 = digitalRead(startBtns[3]);

  if (btn4 != lastBtn_state4) {
    lastDebounceTime4 = millis();
  }
  
  if ((millis() - lastDebounceTime4) > debounceDelay) {
    if (btn4 != btn_state4) {
      btn_state4 = btn4;
    }

    if (btn_state4 == HIGH) {
      Serial.println("btn4");

      systemReady = true;
      anagRead4 = analogRead(potPin);
      val4 = map(anagRead4, 0, 4096, 0, 20);
    }
  }
  
  servoControl();

  if (skipMixer == false && systemReady == true) {
    for(int i = 0; i > 0; i--) {
       pinMode(mixerPin, HIGH);
       delay(1000);

       Serial.println("Mezcladora activa");
    }

    skipMixer = true;
    systemReady = false;
  }
  else {
    pinMode(mixerPin, LOW);
  }

  lastBtn_state = btn1;
  lastBtn_state2 = btn2;
  lastBtn_state3 = btn3;
  lastBtn_state4 = btn4;
}

void servoControl() {
  if (val1 > 0 && val2 > 0 &&  val3 > 0 && val4 > 0) {
    skipMixer = true;
  }
  else {
    skipMixer = false;
  }

  if (val1 > 0) {
    val1--;
    delay(1000);
    
    if (servo1.attached()) {
      servo1.write(90);
      delay(10);
      servo1.detach();
      
      Serial.println("servo 1: active");
    }
  } 
  else if (!servo1.attached()) {
    servo1.setPeriodHertz(50);
    servo1.attach(servoPins[0], minUs, maxUs);
      
    servo1.write(0);
    Serial.println("Servo 1: not active");
  }

  if (val2 > 0) {
    val2--;
    delay(1000);
    
    if (servo2.attached()) {
      servo2.write(90);
      delay(10);
      servo2.detach();
      
      Serial.println("servo 2: active");
    }
  } 
  else if (!servo2.attached()) {
    servo2.setPeriodHertz(50);
    servo2.attach(servoPins[1], minUs, maxUs);
      
    servo2.write(0);
    Serial.println("Servo 2: not active");
  }

  if (val3 > 0) {
    val3--;
    delay(1000);
    
    if (servo3.attached()) {
      servo3.write(90);
      delay(10);
      servo3.detach();
      
      Serial.println("servo 3: active");
    }
  } 
  else if (!servo3.attached()) {
    servo3.setPeriodHertz(50);
    servo3.attach(servoPins[2], minUs, maxUs);
      
    servo3.write(0);
    Serial.println("Servo 3: not active");
  }

  if (val4 > 0) {
    val4--;
    delay(1000);
    
    if (servo4.attached()) {
      servo4.write(90);
      delay(10);
      servo4.detach();
      
      Serial.println("servo 4: active");
    }
  } 
  else if (!servo4.attached()) {
    servo4.setPeriodHertz(50);
    servo4.attach(servoPins[3], minUs, maxUs);
      
    servo4.write(0);
    Serial.println("Servo 4: not active");
  }

// delay(1000);
}