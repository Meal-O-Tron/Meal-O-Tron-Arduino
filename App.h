#ifndef APP_H
#define APP_H

#include "Arduino.h"
#include "ConfigDog.h"
#include "ConfigSchedule.h"
#include "ConfigESP.h"
#include "RTC.h"
#include "SerialUtils.h"
#include "Stats.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

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

    void resetESP();

  private:
    SerialUtils m_serial;
    void checkESPState();

    ConfigDog m_configDog;
    ConfigSchedule m_configSchedule;
    ConfigESP m_configESP;
    Stats m_stats;

    boolean m_distributing;

    RTC m_rtc;
    void onMinuteChanged();
    
    Adafruit_SSD1306 m_oledDisplay;
    boolean m_displayReady;
};

#endif // APP_H
