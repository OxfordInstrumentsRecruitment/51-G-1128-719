#include "definitions.h"

//display state machine variables
uint8_t displayState=DISPLAYSTATE_BLACK;
bool updateDisplayFlag=true;
float p = 3.1415926;

void updateDisplay()
{
  //only update the display if the state changes
  if(!updateDisplayFlag)
    return;

  //only update the display once
  updateDisplayFlag=false;
  schedState = SCHED_TRIGGERED;
  
  //Everything is ok unless a specific error occurs
  displayStatus=STATUS_OK;
  
  switch(displayState)
  {
    case DISPLAYSTATE_NONE:
    {
      break;
    }
    case DISPLAYSTATE_BLACK:
    {
      tft.fillScreen(ST77XX_BLACK);
      break;
    }
    case DISPLAYSTATE_TAG:
    {
      if(SDPresent)
      {
        if(sd.exists("logo.bmp"))
        {
          file = sd.open("logo.bmp", FILE_READ);
          
          tft.fillScreen(ST77XX_BLACK);
          tft.setTextWrap(false);
          
          Draw_BMP_SD(&file, &tft, 0, 0, false);
          file.close();

          tft.setTextSize(4);
          tft.setCursor(0, 140);
          tft.setTextColor(ST77XX_WHITE);
          tft.print(TagName);
          tft.setTextSize(2);
          tft.setCursor(0, 178);
          tft.print(TagPosition);
        }
        else
        {
          //file not found
          displayStatus=STATUS_NOFILE;
        }
      }
      else
      {
        //no SD card inserted
        displayStatus=STATUS_NOSD;
      }
      break;
    }
    case DISPLAYSTATE_IMAGE:
    {
      if(SDPresent)
      {
        char fileName[13];
        sprintf(fileName, "%d.bmp", displayImage);
        if(sd.exists(fileName))
        {
          file = sd.open(fileName, FILE_READ);
          tft.fillScreen(ST77XX_BLACK);
          Draw_BMP_SD(&file, &tft, 0, 0, true);
          file.close();
        }
        else
        {
          //file not found
          displayStatus=STATUS_NOFILE;
        }
      }
      else
      {
        //no SD card inserted
        displayStatus=STATUS_NOSD;
      }
      break;
    }
    case DISPLAYSTATE_LORUM:
    {
      tft.fillScreen(ST77XX_BLACK);
      testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. " \
        "Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, " \
        "fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque " \
        "ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa " \
        "eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere.", ST77XX_WHITE);
      break;
    }
    case DISPLAYSTATE_PRINT:
    {
      tftPrintTest();
      break;
    }
    case DISPLAYSTATE_LINES:
    {
      testlines(ST77XX_YELLOW);
      break;
    }
    case DISPLAYSTATE_RECT:
    {
      testdrawrects(ST77XX_GREEN);
      break;
    }
    case DISPLAYSTATE_FILLRECT:
    {
      testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
      break;
    }
    case DISPLAYSTATE_CIRCLES:
    {
      tft.fillScreen(ST77XX_BLACK);
      testfillcircles(10, ST77XX_BLUE);
      testdrawcircles(10, ST77XX_WHITE);
      break;
    }
    case DISPLAYSTATE_TRIANGLES:
    {
      testtriangles();
      break;
    }
    default:
    {
      return;
    }
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

void testdrawtext(char *text, uint16_t color)
{
  uint8_t prints = 0;
  uint8_t rows =0;

  tft.setTextSize(1);       //each char is 6x8 pixels
  tft.setTextColor(color);
  tft.setTextWrap(true);

  //fill the screen with the available text, calculate the number of times to print it
  //first work out how many rows 1 message take up
  rows = (strlen(text) / (tft.width() / 6));
  //check for a partial row
  if(strlen(text) % (tft.width() / 6))
    rows ++;
  //work out how many sets can fit on the screen (rounded down)
  prints = (tft.height() / 8) / rows;

  for(uint8_t i=0; i<prints; i++)
  {
    tft.setCursor(0, i*rows*8);
    tft.print(text);
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
