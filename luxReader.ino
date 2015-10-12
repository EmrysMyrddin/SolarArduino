#include <Servo.h>

int const luxMax = 150;
int const luxMin = 700;
int const luxPin = 0;

int const potarPin = 5;

int const dataPin = 8;
int const latchPin = 12;
int const clockPin = 10;

Servo servo;
int const servoPin = 3;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  servo.attach(servoPin);
  Serial.begin(9600);
}

bool needToMove = false;
int posServo = 10;
int displayValue = 0;

void loop() {
  int lux = analogRead(luxPin);
  int potar =  analogRead(potarPin);
  
  needToMove = abs(lux - potar) > (needToMove ? 50 : 70);

  int pas = needToMove ? (lux - potar) / abs(lux - potar) : 0;
  posServo += pas;
  posServo = posServo >= 10 ? posServo <= 170 ? posServo : 170 : 10;
  servo.write(posServo);
  
  updateDisplay(pas == -1, pas == 0, pas == 1);
  
  delay(50);
}

void updateDisplay(int left, int midle, int right)
{
  int value;
  
  if(left) value = 32;
  if(midle) value = 8;
  if(right) value = 2;
  
  if(value == displayValue) return;

  Serial.write(value);

  displayValue = value;
  updateRegister(value);
}

void updateRegister(byte value)
{ 
  log(value);
  digitalWrite(latchPin, LOW); //Mode écriture activé, led éteintes
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  Serial.write(value);
  digitalWrite(latchPin, HIGH); //Mode écriture désactivé, led allumées
}

void log(char msg[])
{
  Serial.println(msg);
}

void log(int num)
{
  Serial.println(num);
}
