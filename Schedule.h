#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Arduino.h"

class Schedule {
  public:
    Schedule(unsigned int hour, unsigned int minute, unsigned int ratio, boolean enabled);

    unsigned int getHour();
    void setHour(unsigned int hour);

    unsigned int getMinute();
    void setMinute(unsigned int minute);

    unsigned int getRatio();
    void setRatio(unsigned int ratio);

    boolean isEnabled();
    void setEnabled(boolean enabled);

  private:
    unsigned int m_hour;
    unsigned int m_minute;
    unsigned int m_ratio;
    boolean m_enabled;
};

#endif SCHEDULE_H
