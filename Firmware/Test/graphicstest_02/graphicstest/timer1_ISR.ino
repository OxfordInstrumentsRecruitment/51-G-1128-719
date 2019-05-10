#include "definitions.h"

//RGB led state machine variables
uint16_t ledState=0;

void timer1_ISR(void)
{
  LED_Wheel(ledState);
  ledState++;
  if(ledState > WHEELPOSMAX)
  {
    ledState=0;
  }

  //update the brightness of the LCD backlight
  //pwmWrite(TFT_BACKLIGHT, LCDBrightness);

  //TO-DO: read the state of the buttons & debounce
  
}

void LED_Wheel(uint16_t pos)
{
  uint32_t newPosR,newPosG,newPosB;
  
  if(pos>WHEELPOSMAX)
  {
    pos %= WHEELPOSMAX;
  }
  if(pos < 256)
  {
    //red up
    //green down
    //blue off
    
    //scale for the efficiency of the LEDs and the 16 bit PWM
    newPosR = 65535 - (255*pos);
    newPosG = 39525 + (102*pos);
    newPosB = 65535;
    
    //analogWrite(LED_RED, 255-pos);
    //analogWrite(LED_GREEN, pos);
    //analogWrite(LED_BLUE, 255);
  }
  else if(pos < 512)
  { 
    //red down
    //green off
    //blue up
    
    //scale for the efficiency of the LEDs (red being the worst)
    pos -= 256;
    newPosR = 255*pos;
    newPosG = 65535;
    newPosB = 65535 - (89*pos);
    
    //analogWrite(LED_RED, pos);
    //analogWrite(LED_GREEN, 255);
    //analogWrite(LED_BLUE, 255-pos);
  }
  else
  {
    //red off
    //green up
    //blue down
    
    //scale for the efficiency of the LEDs (red being the worst)
    pos -= 512;
    newPosR = 65535;
    newPosG = 65535 - (102*pos);
    newPosB = 42840 + (89*pos);
    
    //analogWrite(LED_RED, 255);
    //analogWrite(LED_GREEN, 255-pos);
    //analogWrite(LED_BLUE, pos);
  }

  pwmWrite(LED_RED, newPosR);
  pwmWrite(LED_GREEN, newPosG);
  pwmWrite(LED_BLUE, newPosB);
}
