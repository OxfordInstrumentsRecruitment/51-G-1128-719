#include "definitions.h"

uint8_t schedStateNext=0;
uint32_t shedWait=0;
uint32_t shedDelay=0;

void updateSchedule(void)
{

  switch(schedState)
  {
    case SCHED_STOPPED:
    {
      //do nothing, the scheduler isn't running
      break;
    }
    case SCHED_ARMED:
    {
      //wait while the display draws
      break;
    }
    case SCHED_TRIGGERED:
    {
      shedWait = millis() + shedDelay;
      schedState = SCHED_WAIT;
      break;
    }
    case SCHED_WAIT:
    {
      if((millis() >= shedWait) || (displayStatus != STATUS_OK))
      {
        //either the time ran out, or something went wrong
        //either way, move on
        schedState = schedStateNext;
      }
      break;
    }
    case SCHED_TAG:
    {
      if(displayStatus == STATUS_OK)
      {
        displayState = DISPLAYSTATE_TAG;
        updateDisplayFlag=true;
        schedState = SCHED_ARMED;
        schedStateNext = SCHED_IMG;
        displayImage=0;
        shedDelay = 4000;
      }
      else
      {
        schedState = SCHED_PRINT;
      }
      break;
    }
    case SCHED_IMG:
    {
      if(displayStatus == STATUS_OK)
      {
        //display the next image
        displayImage++;
        displayState = DISPLAYSTATE_IMAGE;
        updateDisplayFlag=true;
        schedState = SCHED_ARMED;
        schedStateNext = SCHED_IMG;
        shedDelay = 1000;
      }
      else
      {
        schedState = SCHED_PRINT;
      }
      break;
    }
    case SCHED_PRINT:
    {
      displayState = DISPLAYSTATE_PRINT;
      updateDisplayFlag=true;
      schedState = SCHED_ARMED;
      schedStateNext = SCHED_LINES;
      shedDelay = 1000;
      break;
    }
    case SCHED_LINES:
    {
      displayState = DISPLAYSTATE_LINES;
      updateDisplayFlag=true;
      schedState = SCHED_ARMED;
      schedStateNext = SCHED_FILLRECT;
      shedDelay = 500;
      break;
    }
    case SCHED_FILLRECT:
    {
      displayState = DISPLAYSTATE_FILLRECT;
      updateDisplayFlag=true;
      schedState = SCHED_ARMED;
      schedStateNext = SCHED_TRIANGLES;
      shedDelay = 500;
      break;
    }
    case SCHED_TRIANGLES:
    {
      displayState = DISPLAYSTATE_TRIANGLES;
      updateDisplayFlag=true;
      schedState = SCHED_ARMED;
      schedStateNext = SCHED_CIRCLES;
      shedDelay = 500;
      break;
    }
    case SCHED_CIRCLES:
    {
      displayState = DISPLAYSTATE_CIRCLES;
      updateDisplayFlag=true;
      schedState = SCHED_ARMED;
      schedStateNext = SCHED_TAG;
      shedDelay = 500;
      break;
    }
    default:
    {
      schedState = SCHED_STOPPED;
      break;
    }
  }
}
