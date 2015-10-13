#include <ST7565.h>
#include <Servo.h>

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


bool needToMoveX = false;
bool needToMoveY = false;
int rotX = 50;
int rotY = 50;

Servo servoX;
Servo servoY;

int const servoPinX = 5;
int const servoPinY = 6;

void setup()   { 

  // turn on backlight
  pinMode(BACKLIGHT_LED, OUTPUT);
  digitalWrite(BACKLIGHT_LED, HIGH);

  // initialize and set the contrast to 0x18
  glcd.begin(0x00);
  glcd.st7565_command(CMD_SET_RESISTOR_RATIO | 0x05);
  glcd.clear();
  
  servoX.attach(servoPinX);
  servoY.attach(servoPinY);
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

int updateServoPos(int res1, int res2, bool* needToMove, int* posServo, int debugLine)
{
  *needToMove = abs(res1 - res2) > (*needToMove ? 50 : 80);
  int delta = res1 - res2;
  int absolute = abs(delta);
  int pas = *needToMove ? delta / absolute : 0;

  *posServo += pas;
  *posServo = *posServo >= 10 ? *posServo <= 170 ? *posServo : 170 : 10;
  
  int_to_char("DIFF:", delta, debugLine);
  return *posServo;
}

void lcdDebug(char[] label, int value, line)
{
  char tmp[10];
  int_to_char(label, value, tmp);
  glcd.drawstring(0, line, tmp);
}

void loop()
{
  glcd.clear();
  
  val_res1 = analogRead(1);
  val_res2 = analogRead(2);
  val_res3 = analogRead(3);
  val_res4 = analogRead(4);
  
  servoX.write(updateServoPos(val_res1, val_res2, &needToMoveX, &rotX, 4));
  servoY.write(updateServoPos(val_res3, val_res4, &needToMoveY, &rotY, 5));
  
  lcdDebug("RES1:", val_res1, 0);
  lcdDebug("RES2:", val_res2, 1);
  lcdDebug("RES3:", val_res3, 2);
  lcdDebug("RES4:", val_res4, 3);
  lcdDebug("ROTX:", rotX, 6);
  lcdDebug("ROTY:", rotY, 7);
  
  glcd.display();
  delay(50);
}
