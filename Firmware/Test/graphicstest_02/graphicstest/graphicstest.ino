
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "SdFat.h"
#include "definitions.h"

// For 1.54" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//declare timer 1 object
HardwareTimer timer(1);

//SD card on SPI2
SPIClass SPI_2(2);
SdFat sd(&SPI_2);

uint8_t displayImage=0;
uint8_t displayStatus=STATUS_OK;
uint8_t schedState=SCHED_TAG;
bool SDPresent = false;
uint16_t LCDBrightness = LCD_DEFAULT_BRIGHTNESS;
char TagName[14] = "Alex Wood";
char TagPosition[21] = "Electronic Engineer";
File file;

void setup(void)
{
  //configure timer 1 to call an ISR every 10 milliseconds
  //used to drive the RGB led around its colour wheel
  //used to update LCD backlight
  //used to capture state of buttons and debounce
  timer.pause();
  timer.setPeriod(LED_RATE);
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
  timer.attachCompare1Interrupt(timer1_ISR);

  //init the display - fill with black before turning back light on
  tft.init(240, 240);           // Init ST7789 240x240
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
  //pinMode(TFT_BACKLIGHT, PWM);
  //pwmWrite(TFT_BACKLIGHT, LCDBrightness); // Backlight on at default brightness

  //set up the LED pins for open drain operation
  pinMode(LED_RED, PWM_OPEN_DRAIN);
  pinMode(LED_GREEN, PWM_OPEN_DRAIN);
  pinMode(LED_BLUE, PWM_OPEN_DRAIN);
  //turn the LEDs off for now
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);

  //SD card socket has a detect in
  pinMode(SD_DET, INPUT_PULLUP);
  
  //start timer 1
  timer.refresh();
  timer.resume();
}

void loop()
{
  //check for the SD card and initialise it if it is present
  checkSDCard();

  //Drive the scheduler state machine along (non-blocking)
  updateSchedule();

  //drive the display state machine along (non-blocking)
  updateDisplay();
}

void checkSDCard(void)
{
  //check for the SD card and initialise it if it is present
  if(digitalRead(SD_DET) == 0)
  {
    if(SDPresent==false)
    {
      if(sd.begin(SD_CS, SD_SCK_MHZ(50)))
      {
        SDPresent = true;

        //get the tag data from the card
        getTagData();
      }
    }
  }
  else
  {
    SDPresent = false;
  }
}

void getTagData(void)
{
  char temp;
  
  if(sd.exists("name.txt"))
  {
    file = sd.open("name.txt", FILE_READ);

    //grab the name from the file
    for(uint8_t i=0; i<14; i++)
    {
      temp = file.read();
      if(temp == '\r')
      {
        //reached the end of the name
        TagName[i] = 0;
        break;
      }
      else
      {
        TagName[i] = temp;
      }
    }

    //ensure there is a null terminator
    TagName[13] = 0;

    //if the name was longer than 13 characters, skip to the next line
    while(temp != '\r')
    {
      temp = file.read();
    }
    //skip over the LF is present
    if(file.peek() == '\n')
    {
      temp = file.read();
    }

    //grab the name from the file
    for(uint8_t i=0; i<21; i++)
    {
      temp = file.read();
      if(temp == '\r')
      {
        //reached the end of the name
        TagPosition[i] = 0;
        break;
      }
      else
      {
        TagPosition[i] = temp;
      }
    }

    //ensure there is a null terminator
    TagPosition[21] = 0;
  }
}
