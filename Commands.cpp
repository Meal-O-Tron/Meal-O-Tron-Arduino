#include <ArduinoJson.h>
#include "Commands.h"

void Commands::processCommand(String *command, HardwareSerial *interface) {
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(*command);

  if (root.success()) {
    if (root["type"].is<int>() && root["data"].is<JsonObject>()) {
      RequestType rqt = root["type"].as<int>();

      switch (rqt) {
        case DATA_GLOBAL_RELOAD: {
          break;
        }
        case DATA_STATS_WEIGHT: {
          break;
        }
        case DATA_STATS_REMAINING_FOOD: {
          break;
        }
        case DATA_STATS_DOG_ARRIVAL: {
          break;
        }
        case DATA_SCHEDULE_ADD: {
          break;
        }
        case DATA_SCHEDULE_REMOVE: {
          break;
        }
        case DATA_SCHEDULE_ENABLE: {
          break;
        }
        case DATA_SCHEDULE_DATE: {
          break;
        }
        case DATA_SCHEDULE_RATIO: {
          break;
        }
        case DATA_DOG_NAME: {
          break;
        }
        case DATA_DOG_WEIGHT: {
          break;
        }
        case DATA_DOG_REGULATION_ENABLE: {
          break;
        }
        case DATA_DOG_REGULATION_VALUE: {
          break;
        }
        default: {
          if (rqt < DATA_GLOBAL_RELOAD || rqt > DATA_DOG_END)
            sendError(interface, -3);
            
          break;
        }
      }
    } else {
      sendError(interface, -2);
    }
  } else {
    sendError(interface, -1);
  }
}

void Commands::sendError(HardwareSerial *interface, int err) {
  interface->println("{\"error\": " + String(err) + "}");
}
