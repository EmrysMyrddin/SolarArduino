#include <ST7565.h>

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

int oldkey=-1;
int px = 0;
int py = 0;
char msg[] = "HELLO WORLD";

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2
int get_key(unsigned int input)
{   
    if (input < 100) return 0;
    else  if (input < 300) return 1;
    else  if (input < 500) return 2;
    else  if (input < 700) return 3;
    else  if (input < 900) return 4;    
    else  return -1;
}

void setup()   { 

  // turn on backlight
  pinMode(BACKLIGHT_LED, OUTPUT);
  digitalWrite(BACKLIGHT_LED, HIGH);

  // initialize and set the contrast to 0x18
  glcd.begin(0x00);
  glcd.st7565_command(CMD_SET_RESISTOR_RATIO | 0x05);
  glcd.clear();
  glcd.drawstring(px, py, msg);
  glcd.display();
}


void loop()                     
{
  unsigned int adc_key_in = analogRead(0);
  int key = get_key(adc_key_in);
  if(key != oldkey)
  {
    if(key == 3)
    {
      px += 1;
      glcd.clear();
      glcd.drawstring(px, py, msg);
      glcd.display();
    }
  }
  oldkey = key;
  delay(20);
}
