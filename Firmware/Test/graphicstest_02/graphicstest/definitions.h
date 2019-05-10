#ifndef DEFINITIONS
#define DEFINITIONS

//display state machine states
enum
{
  DISPLAYSTATE_NONE,
  DISPLAYSTATE_BLACK,
  DISPLAYSTATE_TAG,
  DISPLAYSTATE_IMAGE,
  DISPLAYSTATE_LORUM,
  DISPLAYSTATE_PRINT,
  DISPLAYSTATE_LINES,
  DISPLAYSTATE_RECT,
  DISPLAYSTATE_FILLRECT,
  DISPLAYSTATE_CIRCLES,
  DISPLAYSTATE_TRIANGLES,
};

//general purpose return states
enum
{
  STATUS_OK,
  STATUS_ERROR,
  STATUS_NOFILE,
  STATUS_NOSD,
  STATUS_FORMATERR,
};

//scheduler states
enum
{
  SCHED_STOPPED,
  SCHED_ARMED,
  SCHED_TRIGGERED,
  SCHED_WAIT,
  SCHED_TAG,
  SCHED_IMG,
  SCHED_PRINT,
  SCHED_LINES,
  SCHED_FILLRECT,
  SCHED_TRIANGLES,
  SCHED_CIRCLES,
};



//pin definitions
#define TFT_CS        PA4
#define TFT_RST       PB4
#define TFT_DC        PB5
#define TFT_BACKLIGHT PA8
#define LED_RED       PB9
#define LED_GREEN     PB8
#define LED_BLUE      PB1
#define SD_DET        PB3
#define SW_LEFT       PC13
#define SW_RIGHT      PC14
#define SW_ENTER      PC15
#define SD_CS         PB12

#define WHEELPOSMAX   767
#define LED_RATE 10000    // in microseconds
#define LCD_DEFAULT_BRIGHTNESS 65535


//------------------------------------------------------------------------------
// print error msg, any SD error codes, and halt.
// store messages in flash
#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))
//------------------------------------------------------------------------------

#endif
