#include "TimeHelper.h"


TimeHelper::TimeHelper(void)
{
}


TimeHelper::~TimeHelper(void)
{
}

// static 
FILETIME TimeHelper::addSeconds (const FILETIME& time, DWORD seconds) {
  // http://stackoverflow.com/questions/5118411/modifying-the-date-and-time-values-using-systemtime-filetime-and-ularge-intege

  ULARGE_INTEGER u  ; 
  memcpy( &u  , &time , sizeof( u ) );

  const double c_dSecondsPer100nsInterval = 100.*1.e-9;
  const double c_dNumberOf100nsIntervals = 
                  seconds / c_dSecondsPer100nsInterval;

  // note: you may want to round the number of intervals.
  u.QuadPart += c_dNumberOf100nsIntervals;

  FILETIME newTime;
  memcpy( &newTime, &u, sizeof( newTime ) );
  return newTime;
}

// static 
ULONGLONG TimeHelper::subtruct(const FILETIME& ftLeft,const FILETIME& ftRight)
{
  ULARGE_INTEGER ul1;
  ul1.LowPart = ftLeft.dwLowDateTime;
  ul1.HighPart = ftLeft.dwHighDateTime;

  ULARGE_INTEGER ul2;
  ul2.LowPart = ftRight.dwLowDateTime;
  ul2.HighPart = ftRight.dwHighDateTime;

  ul1.QuadPart -= ul2.QuadPart;

  ULARGE_INTEGER uliRetValue;
  uliRetValue.QuadPart = 0;
  uliRetValue = ul1;
  uliRetValue.QuadPart /= 10;
  //uliRetValue.QuadPart /= 1000;

  return uliRetValue.QuadPart;
}
