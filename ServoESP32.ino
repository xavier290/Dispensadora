#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo1, servo2, servo3, servo4, servo5;
LiquidCrystal_I2C lcd(0x27, 20, 4);

long duration[] = {0, 0, 0, 0};
long distance[] = {0, 0, 0, 0};
long nivel[] = {0, 0, 0, 0};

int echoPins[] = {22, 24, 26, 28};
int trigPins[] = {30, 32, 34, 36};

struct Button {
  const uint8_t PIN;
  volatile bool PRESSED;
  volatile long VALUE;
};

Button btn1 = {18, false};
Button btn2 = {19, false};
Button btn3 = {2, false};
Button btn4 = {3, false};

int servoPins[] = {4, 5, 6, 7, 8};
int mixer = 38;

long timer;
long val1, val2, val3, val4;
bool systemReady = false;

int mixerTimeActive = 20;

static unsigned long last_interrupt_time = 0;

void my_interrupt_handler(Button &theButton) {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 400ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 400) {
    theButton.PRESSED = true;
    theButton.VALUE = analogRead(A0);
  }

  last_interrupt_time = interrupt_time;
}

void ISR1() {
  my_interrupt_handler(btn1);
}

void ISR2() {
  my_interrupt_handler(btn2);
}

void ISR3() {
  my_interrupt_handler(btn3);
}

void ISR4() {
  my_interrupt_handler(btn4);
}

void setup() {
  servo1.attach(servoPins[0], 1000, 2000);
  servo2.attach(servoPins[1], 1000, 2000);
  servo3.attach(servoPins[2], 1000, 2000);
  servo4.attach(servoPins[3], 1000, 2000);
  servo5.attach(servoPins[4], 1000, 2000);

  Serial.begin(9600);

  pinMode(btn1.PIN, INPUT_PULLUP);
  pinMode(btn2.PIN, INPUT_PULLUP);
  pinMode(btn3.PIN, INPUT_PULLUP);
  pinMode(btn4.PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(18), ISR1, FALLING);
  attachInterrupt(digitalPinToInterrupt(19), ISR2, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), ISR3, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), ISR4, FALLING);

  for(int i = 0; i < 5; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  
  pinMode(mixer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Buen dia! :)");

  delay(1000);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);
}

void loop() {
  // Serial.println(analogRead(A0));
  if (btn1.PRESSED) {
    Serial.print("Btn Pressed ");
    btn1.PRESSED = false;

    val1 = map(btn1.VALUE, 0, 1024, 0, 20);
    Serial.println(val1);
    systemReady = true;

    servoControl(servo1, val1);
  }
  else if (btn2.PRESSED) {
    Serial.print("Btn2 Pressed ");
    btn2.PRESSED = false;
    systemReady = true;

    val2 = map(btn2.VALUE, 0, 1024, 0, 20);
    Serial.println(val2);

    servoControl(servo2, val2);
  }
  else if (btn3.PRESSED) {
    Serial.print("Btn3 Pressed ");
    btn3.PRESSED = false;
    systemReady = true;

    val3 = map(btn3.VALUE, 0, 1024, 0, 20);
    Serial.println(val3);

    servoControl(servo3, val3);
  }
  else if (btn4.PRESSED) {
    Serial.print("Btn4 Pressed ");
    btn4.PRESSED = false;
    systemReady = true;

    val4 = map(btn4.VALUE, 0, 1024, 0, 20);
    Serial.println(val4);

    servoControl(servo4, val4);
  }

  if(systemReady && !btn1.PRESSED && !btn2.PRESSED && !btn3.PRESSED && !btn4.PRESSED && mixerTimeActive >= 0) {
    digitalWrite(mixer, HIGH);
    mixerTimeActive--;

    Serial.println("Mixer active");

    if(mixerTimeActive == 0) {
      mixerTimeActive = 20;
      systemReady = false;

      digitalWrite(mixer, LOW);
      for(int i = 0; i < 15; i++) {
        servo5.write(180);
        Serial.println("Abierto");
        delay(1000);
      }

      servo5.write(0);
    }
  }
  else {
    digitalWrite(mixer, LOW);
  }

 measureStorage();
}

void servoControl(Servo &theServo, int val) {
  while(val > 0) {
    digitalWrite(mixer, LOW);

    theServo.write(180);
    delay(1000);

    val--;
    Serial.println("Running ");
  }

  theServo.write(0);
}

void measureStorage() {
  digitalWrite(trigPins[0], LOW);

  delayMicroseconds(2);
  digitalWrite(trigPins[0], HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPins[0], LOW);
  duration[0] = pulseIn(echoPins[0], HIGH);
  distance[0] = duration[0] * 0.034 / 2;

  nivel[0] = map(distance[0], 4, 18, 100, 0);

  digitalWrite(trigPins[1], LOW);

  delayMicroseconds(2);
  digitalWrite(trigPins[1], HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPins[1], LOW);
  duration[1] = pulseIn(echoPins[1], HIGH);
  distance[1] = duration[1] * 0.034 / 2;

  nivel[1] = map(distance[1], 4, 18, 100, 0);

  digitalWrite(trigPins[2], LOW);

  delayMicroseconds(2);
  digitalWrite(trigPins[2], HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPins[2], LOW);
  duration[2] = pulseIn(echoPins[2], HIGH);
  distance[2] = duration[2] * 0.034 / 2;

  nivel[2] = map(distance[2], 4, 18, 100, 0);

  digitalWrite(trigPins[3], LOW);

  delayMicroseconds(2);
  digitalWrite(trigPins[3], HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPins[3], LOW);
  duration[3] = pulseIn(echoPins[3], HIGH);
  distance[3] = duration[3] * 0.034 / 2;

  nivel[3] = map(distance[3], 4, 18, 100, 0);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("A1: ");
  lcd.setCursor(4, 0);
  lcd.print(nivel[0]);

  lcd.setCursor(0, 1);
  lcd.print("A2: ");
  lcd.setCursor(4, 1);
  lcd.print(nivel[1]);

  lcd.setCursor(8, 0);
  lcd.print("A3: ");
  lcd.setCursor(12, 0);
  lcd.print(nivel[2]);

  lcd.setCursor(8, 1);
  lcd.print("A4: ");
  lcd.setCursor(12, 1);
  lcd.print(nivel[3]);

  delay(1000);
}