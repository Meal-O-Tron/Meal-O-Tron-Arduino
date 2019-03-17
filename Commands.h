#ifndef COMMANDS_H
#define COMMANDS_H

#include "Arduino.h"
#include <ArduinoJson.h>
#include "App.h"

class Commands {
  public:
    enum RequestType {
      DATA_GLOBAL_RELOAD,
      
      DATA_STATS_START,
      DATA_STATS_WEIGHT,
      DATA_STATS_REMAINING_FOOD,
      DATA_STATS_DOG_ARRIVAL,
      DATA_STATS_END,
  
      DATA_SCHEDULE_START,
      DATA_SCHEDULE_ADD,
      DATA_SCHEDULE_REMOVE,
      DATA_SCHEDULE_ENABLE,
      DATA_SCHEDULE_DATE,
      DATA_SCHEDULE_RATIO,
      DATA_SCHEDULE_END,
  
      DATA_DOG_START,
      DATA_DOG_NAME,
      DATA_DOG_WEIGHT,
      DATA_DOG_REGULATION_ENABLE,
      DATA_DOG_REGULATION_VALUE,
      DATA_DOG_END,

      ESP_CONFIG_REQUEST = 100,
      ESP_UPDATE_TIME,
    };

    static int processCommand(String *command, HardwareSerial *interface, App *app);

  private:
    static void sendError(Print *interface, int err);
};

#endif // COMMANDS_H
