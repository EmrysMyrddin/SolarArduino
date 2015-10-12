#include <ST7565.h>
#include <Servo.h>

int ledPin =  13;    // LED connected to digital pin 13

// the LCD backlight is connected up to a pin so you can turn it on & off
#define BACKLIGHT_LED 7

// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 8 - LCD reset (RST)
// pin 10 - LCD chip select (CS)
ST7565 glcd(11, 13, 9, 8, 10);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

unsigned int val_res1 = 1000;
unsigned int val_res2 = 750;
unsigned int val_res3 = 500;
unsigned int val_res4 = 250;

int rot1 = 20;
int rot2 = -1;

Servo servo1;
Servo servo2;

int const servoPin1 = 5;
int const servoPin2 = 6;

void setup()   { 

  // turn on backlight
  pinMode(BACKLIGHT_LED, OUTPUT);
  digitalWrite(BACKLIGHT_LED, HIGH);

  // initialize and set the contrast to 0x18
  glcd.begin(0x00);
  glcd.st7565_command(CMD_SET_RESISTOR_RATIO | 0x05);
  glcd.clear();
  
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void int_to_char(char* in, int val, char* out)
{
  String str = String(in);
  String v = String(val);
  String res = String("");
  res += str;
  res += v;
  
  res.toCharArray(out,10);
}

bool needToMove = false;
int posServo = 10;

void loop()
{
  val_res1 = analogRead(1);
  val_res2 = analogRead(2);
  
  needToMove = abs(val_res1 - val_res2) > (needToMove ? 20 : 40);
  
  int delta = (val_res1 - val_res2);
  int div = abs(delta);
  
  int pas = needToMove ?  delta / div : 0;
  posServo += pas;
  posServo = posServo >= 10 ? posServo <= 170 ? posServo : 170 : 10;
  
  servo1.write(posServo);
  servo2.write(posServo);
  
  char tmp[10];
  glcd.clear();
  
  int_to_char("RES1:", val_res1, tmp);
  glcd.drawstring(0, 0, tmp);
  
  int_to_char("RES2:", val_res2, tmp);
  glcd.drawstring(0, 1, tmp);
  
  int_to_char("RES3:", pas, tmp);
  glcd.drawstring(0, 2, tmp);
  
  int_to_char("RES4:", val_res4, tmp);
  glcd.drawstring(0, 3, tmp);
  
  int_to_char("ROT1:", posServo, tmp);
  glcd.drawstring(0, 5, tmp);
  
  int_to_char("ROT2:", rot2, tmp);
  glcd.drawstring(0, 6, tmp);
  
  glcd.display();
  delay(10);
}
