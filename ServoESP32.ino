#include <ESP32Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int minUs = 1000;
int maxUs = 2000;

int servoPins[] = {4, 2, 5, 14};

struct Button {
  const uint8_t PIN;
  bool PRESSED;
};

// GPIO pins for servo motors and btns
Button btn1 = {25, false};
Button btn2 = {26, false};
Button btn3 = {32, false};
Button btn4 = {33, false};

// GPIO pin for the mixer motor and potentiometer || Pin GPIO para la mexcladora
int mixer = 21;
int potPin = 12;

int anagRead1, anagRead2, anagRead3, anagRead4;  // Variables to read the values from the analog pins (potentiometers connected)
int val1, val2, val3, val4; //Variables to set as a timer for the motors to move at a direction

bool systemReady;
bool skipMixer = false;

static unsigned long last_interrupt_time = 0;

void my_interrupt_handler(Button &theButton) {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
    theButton.PRESSED = true;
  }

  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR ISR1() {
  my_interrupt_handler(btn1);
}

void IRAM_ATTR ISR2() {
  my_interrupt_handler(btn2);
}

void IRAM_ATTR ISR3() {
  my_interrupt_handler(btn3);
}

void IRAM_ATTR ISR4() {
  my_interrupt_handler(btn4);
}

void setup() {
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  Serial.begin(115200);

  servo1.setPeriodHertz(50);      // Standard 50hz servo
	servo2.setPeriodHertz(50);      
	servo3.setPeriodHertz(50);      
	servo4.setPeriodHertz(50);      

  servo1.attach(servoPins[0], minUs, maxUs);
	servo2.attach(servoPins[1], minUs, maxUs);
  servo3.attach(servoPins[2], minUs, maxUs);
  servo4.attach(servoPins[3], minUs, maxUs);

  pinMode(btn1.PIN, INPUT_PULLUP);
  pinMode(btn2.PIN, INPUT_PULLUP);
  pinMode(btn3.PIN, INPUT_PULLUP);
  pinMode(btn4.PIN, INPUT_PULLUP);

  attachInterrupt(btn1.PIN, ISR1, HIGH);
  attachInterrupt(btn2.PIN, ISR2, HIGH);
  attachInterrupt(btn3.PIN, ISR3, HIGH);
  attachInterrupt(btn4.PIN, ISR4, HIGH);
}

void loop() {
  if (btn1.PRESSED) {
    Serial.print("Btn Pressed");

    btn1.PRESSED = false;
    systemReady = true;
    anagRead1 = analogRead(potPin);
    val1 = map(anagRead1, 0, 4096, 0, 20);

    servoControl(servo1, val1);
  }
  else if (btn2.PRESSED) {
    Serial.println("Btn2 Pressed");

    btn2.PRESSED = false;
    systemReady = true;
    anagRead2 = analogRead(potPin);
    val2 = map(anagRead2, 0, 4096, 0, 20);

    servoControl(servo2, val2);
  }
  else if (btn3.PRESSED) {
    Serial.println("Btn3 Pressed");

    btn3.PRESSED = false;
    systemReady = true;
    anagRead3 = analogRead(potPin);
    val3 = map(anagRead3, 0, 4096, 0, 20);

    servoControl(servo3, val3);
  }
  else if (btn4.PRESSED) {
    Serial.println("Btn4 Pressed");

    btn4.PRESSED = false;
    systemReady = true;
    anagRead4 = analogRead(potPin);
    val4 = map(anagRead4, 0, 4096, 0, 20);

    servoControl(servo4, val4);
  }
}

void servoControl(Servo &theServo, int val) {
  while(val > 0) {
    theServo.write(90);
    delay(1000);

    val--;
  }

  theServo.write(0);
}