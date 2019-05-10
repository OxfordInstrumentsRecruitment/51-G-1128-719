#include "definitions.h"

//enough memory for 1 line of a 240x240 display
uint8_t sdBuffer[730];

uint8_t Draw_BMP_SD(  File *file, Adafruit_SPITFT *tft, uint8_t x, uint16_t y, bool centre)
{
  uint32_t bmpImageSize;
  uint32_t bmpImageOffset;
  int32_t  bmpWidth;
  int32_t  bmpHeight;
  uint32_t rowSize;
  uint16_t bmpDepth;
  bool     flip = true;
  int32_t  w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint16_t BMPReadSize=0;
  //uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer

  //check that there is a bit map file
  if(!file)
  {
    return STATUS_NOFILE;
  }

  //check that the file is a bit map
  if(read16(file) != 0x4D42)
  {
    //Serial.println(F("[Draw_BMP_SD] Incorrect magic number"));
    return STATUS_FORMATERR;
  }

  //parse the BMP header
  bmpImageSize = read32(file);     // Read the file length
  //Serial.println(F("[Draw_BMP_SD] Image size: "));
  //Serial.println(bmpImageSize, DEC);
  
  (void)read32(file);              // Read & ignore creator bytes
  bmpImageOffset = read32(file);   // Start of image data
  //Serial.println(F("[Draw_BMP_SD] Image offset: "));
  //Serial.println(bmpImageOffset, DEC);

  //parse the DIB header
  (void)read32(file);              // Read & ignore the header size
  bmpWidth  = read32(file);
  bmpHeight = read32(file);

  //Serial.print(F("[Draw_BMP_SD] Image size: "));
  //Serial.print(bmpWidth);
  //Serial.print('x');
  //Serial.println(bmpHeight);

  if(read16(file) != 1)
  {
    //bitmap not in a supported format
    //Serial.println(F("[Draw_BMP_SD] BMP format not supported"));
    return STATUS_FORMATERR;
  }

  if(read16(file) != 24)
  {
    //Image must be 24 bits per pixel
    //Serial.print(F("[Draw_BMP_SD] Colour depth wrong"));
    return STATUS_FORMATERR;
  }

  if(read32(file) != 0)
  {
    //image must not be compressed
    //Serial.println(F("[Draw_BMP_SD] Image is compressed, can't display it"));
    return STATUS_FORMATERR;
  }

  //image checks out, time to display it
  // BMP rows are padded (if needed) to 4-byte boundary
  rowSize = (bmpWidth * 3 + 3) & ~3;

  // If bmpHeight is negative, image is in top-down order.
  // This is not canon but has been observed in the wild.
  if(bmpHeight < 0)
  {
    bmpHeight = -bmpHeight;
    flip = false;
  }

  if(centre)
  {
    //centre the image and disregard the drawing coordinates
    if(bmpWidth < tft->width())
    {
      x = (tft->width() - bmpWidth) / 2;
    }
    if(bmpHeight < tft->height())
    {
      y = (tft->height() - bmpHeight) / 2;
    }
  }
  
  // Crop area to be loaded
  w = bmpWidth;
  h = bmpHeight;
  if((x+w-1) >= tft->width())  w = tft->width()  - x;
  if((y+h-1) >= tft->height()) h = tft->height() - y;
  BMPReadSize = w * 3;

  // Set TFT address window to clipped image bounds
  tft->startWrite();
  tft->setAddrWindow(x, y, w, h);

  //scan through the file row by row
  for (row=0; row<h; row++)
  {
    if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
      pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;
    else     // Bitmap is stored top-to-bottom
      pos = bmpImageOffset + row * rowSize;

    //seek through the file to the start of the current row
    if(file->position() != pos)
    {
      file->seek(pos);
    }

    //read a line from the file
    file->read(sdBuffer, BMPReadSize);

    //convert from 24-bit to 16-bit and send to the screen
    uint16_t buffPTR=0;
    for(uint8_t i=0; i<w; i++)
    {
      tft->SPI_WRITE16(tft->color565(sdBuffer[buffPTR+2], sdBuffer[buffPTR+1], sdBuffer[buffPTR]));
      buffPTR += 3;
    }
  }

  tft->endWrite();
  
  return STATUS_OK;
}

uint16_t read16(File *f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read(); // MSB
  return result;
}

uint32_t read32(File *f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read();
  ((uint8_t *)&result)[2] = f->read();
  ((uint8_t *)&result)[3] = f->read(); // MSB
  return result;
}
