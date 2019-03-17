#ifndef RTC_H
#define RTC_H

#include <DS3231.h>
#include <Wire.h>
#include "Arduino.h"

class RTC {
  public:
    void begin();
    void update();
    
    DateTime getCurrentDate();
    DateTime getPreviousDate();

  private:
    DS3231 m_rtc;

    DateTime m_currentDate;
    DateTime m_previousDate;
};

#endif // RTC_H
