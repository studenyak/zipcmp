#pragma once
#include <Windows.h>

class TimeHelper
{
public:
  TimeHelper(void);
  ~TimeHelper(void);

  static FILETIME addSeconds (const FILETIME& time, DWORD seconds);
  static ULONGLONG subtruct(const FILETIME& ftLeft,const FILETIME& ftRight);
};

