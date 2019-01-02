#ifndef APP_H
#define APP_H

#include "Arduino.h"
#include "ConfigDog.h"
#include "ConfigSchedule.h"
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
    Stats* getStats();

    void saveConfigDog();
    void saveConfigSchedule();

  private:
    SerialUtils m_serial;

    ConfigDog m_configDog;
    ConfigSchedule m_configSchedule;
    Stats m_stats;

    RTC m_rtc;
};

#endif // APP_H
