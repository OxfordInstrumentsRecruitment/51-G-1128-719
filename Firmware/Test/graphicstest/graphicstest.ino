
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//pin definitions
#define TFT_CS        PA4
#define TFT_RST       PB4
#define TFT_DC        PB5
#define TFT_BACKLIGHT PA8
#define LED_RED       PB9
#define LED_GREEN     PB8
#define LED_BLUE      PB1
#define WHEELPOSMAX   767

//display state machine states
enum
{
  DISPLAYSTATE_WAIT,
  DISPLAYSTATE_LORUM,
  DISPLAYSTATE_PRINT,
  DISPLAYSTATE_LINES,
  DISPLAYSTATE_FASTLINES,
  DISPLAYSTATE_RECT,
  DISPLAYSTATE_FILLRECT,
  DISPLAYSTATE_CIRCLES,
  DISPLAYSTATE_ROUNDRECT,
  DISPLAYSTATE_TRIANGLES,
};

// For 1.54" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void)
{
  //init the display - fill with black before turning back light on
  tft.init(240, 240);           // Init ST7789 240x240
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

  //set up the LED pins for open drain operation
  pinMode(LED_RED, PWM_OPEN_DRAIN);
  pinMode(LED_GREEN, PWM_OPEN_DRAIN);
  pinMode(LED_BLUE, PWM_OPEN_DRAIN);
  //turn the LEDs off for now
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
}

uint32_t ledTimer=0;
uint16_t ledState=0;

void loop()
{
  //drive the display state machine along
  updateDisplay();

  //update the RGB led every 10ms
  if(millis() >= ledTimer)
  {
    LED_Wheel(ledState);
    ledState++;
    if(ledState > WHEELPOSMAX)
    {
      ledState=0;
    }
    ledTimer = millis() + 10;
  }
}

//display state machine variables
uint8_t displayState=DISPLAYSTATE_LORUM;
uint8_t displayStateNext=0;
uint32_t displayWait=0;

void updateDisplay()
{
  switch(displayState)
  {
    case DISPLAYSTATE_WAIT:
    {
      if(millis() >= displayWait)
      {
        displayState = displayStateNext;
      }
      break;
    }
    case DISPLAYSTATE_LORUM:
    {
      tft.fillScreen(ST77XX_BLACK);
      testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
      displayWait = millis() + 2000;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_PRINT;
      break;
    }
    case DISPLAYSTATE_PRINT:
    {
      tftPrintTest();
      displayWait = millis() + 4000;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_LINES;
      break;
    }
    case DISPLAYSTATE_LINES:
    {
      testlines(ST77XX_YELLOW);
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_FASTLINES;
      break;
    }
    case DISPLAYSTATE_FASTLINES:
    {
      testfastlines(ST77XX_RED, ST77XX_BLUE);
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_RECT;
      break;
    }
    case DISPLAYSTATE_RECT:
    {
      testdrawrects(ST77XX_GREEN);
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_FILLRECT;
      break;
    }
    case DISPLAYSTATE_FILLRECT:
    {
      testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_CIRCLES;
      break;
    }
    case DISPLAYSTATE_CIRCLES:
    {
      tft.fillScreen(ST77XX_BLACK);
      testfillcircles(10, ST77XX_BLUE);
      testdrawcircles(10, ST77XX_WHITE);
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_ROUNDRECT;
      break;
    }
    case DISPLAYSTATE_ROUNDRECT:
    {
      testroundrects();
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_TRIANGLES;
      break;
    }
    case DISPLAYSTATE_TRIANGLES:
    {
      testtriangles();
      displayWait = millis() + 500;
      displayState = DISPLAYSTATE_WAIT;
      displayStateNext = DISPLAYSTATE_LORUM;
      break;
    }
    default:
    {
      return;
    }
  }
}

void LED_Wheel(uint16_t pos)
{
  if(pos>WHEELPOSMAX)
  {
    pos %= WHEELPOSMAX;
  }
  if(pos < 256)
  {
    analogWrite(LED_RED, 255-pos);
    analogWrite(LED_GREEN, pos);
    analogWrite(LED_BLUE, 255);
  }
  else if(pos < 512)
  {
    pos -= 256;
    analogWrite(LED_RED, pos);
    analogWrite(LED_GREEN, 255);
    analogWrite(LED_BLUE, 255-pos);
  }
  else
  {
    pos -= 512;
    analogWrite(LED_RED, 255);
    analogWrite(LED_GREEN, 255-pos);
    analogWrite(LED_BLUE, pos);
  }
}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}
