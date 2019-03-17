#include "RTC.h"

void RTC::begin() {
  Wire.begin();
}

void RTC::update() {
  bool h12, isPM, century;

  m_previousDate = m_currentDate;
  m_currentDate = DateTime(m_rtc.getYear(), m_rtc.getMonth(century), m_rtc.getDate(), m_rtc.getHour(h12, isPM), m_rtc.getMinute(), m_rtc.getSecond());
}

DateTime RTC::getCurrentDate() {
  return m_currentDate;
}

DateTime RTC::getPreviousDate() {
  return m_previousDate;
}

void RTC::setTime(int year, int month, int day, int dow, int hour, int minute, int second) {
  m_rtc.setYear(year);
  m_rtc.setMonth(month);
  m_rtc.setDate(day);
  m_rtc.setDoW(dow);
  m_rtc.setHour(hour);
  m_rtc.setMinute(minute);
  m_rtc.setSecond(second);
}
