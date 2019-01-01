#ifndef CONFIGSCHEDULE_H
#define CONFIGSCHEDULE_H

#include "Arduino.h"
#include "Schedule.h"
#include <ArduinoJson.h>
#include <LinkedList.h>

class ConfigSchedule {
  public:
    ConfigSchedule();

    void loadConfig(JsonArray *config);
    String generateConfig();

    LinkedList<Schedule*>* getScheduleList();

  private:
    LinkedList<Schedule*> m_scheduleList;
};

#endif // CONFIGSCHEDULE_H
