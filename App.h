#ifndef APP_H
#define APP_H

#include "Arduino.h"
#include "ConfigDog.h"
#include "ConfigSchedule.h"
#include "ConfigESP.h"
#include "RTC.h"
#include "SerialUtils.h"
#include "Stats.h"

class App {
  public:
    App();
    
    void setup();
    void loop();

    ConfigDog* getConfigDog();
    ConfigSchedule* getConfigSchedule();
    ConfigESP* getConfigESP();
    Stats* getStats();
    RTC* getRTC();

    void saveConfigDog();
    void saveConfigSchedule();
    void saveConfigESP();

  private:
    SerialUtils m_serial;
    void checkESPState();

    ConfigDog m_configDog;
    ConfigSchedule m_configSchedule;
    ConfigESP m_configESP;
    Stats m_stats;

    RTC m_rtc;
    void onMinuteChanged();
};

#endif // APP_H
