#include "Schedule.h"

Schedule::Schedule(unsigned int hour, unsigned int minute, unsigned int ratio, boolean enabled)
: m_hour(hour)
, m_minute(minute)
, m_ratio(ratio)
, m_enabled(enabled)
{
  
}


unsigned int Schedule::getHour() {
  return m_hour;
}

void Schedule::setHour(unsigned int hour) {
  m_hour = hour;
}

unsigned int Schedule::getMinute() {
  return m_minute;
}

void Schedule::setMinute(unsigned int minute) {
  m_minute = minute;
}

unsigned int Schedule::getRatio() {
  return m_ratio;
}

void Schedule::setRatio(unsigned int ratio) {
  m_ratio = ratio;
}

boolean Schedule::isEnabled() {
  return m_enabled;
}

void Schedule::setEnabled(boolean enabled) {
  m_enabled = enabled;
}
