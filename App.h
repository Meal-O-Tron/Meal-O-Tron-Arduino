#ifndef APP_H
#define APP_H

#include "Arduino.h"
#include "ConfigDog.h"
#include "ConfigSchedule.h"
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

  private:
    SerialUtils m_serial;

    ConfigDog m_configDog;
    ConfigSchedule m_configSchedule;
    Stats m_stats;
};

#endif // APP_H