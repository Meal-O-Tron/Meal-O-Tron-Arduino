#include "Commands.h"

int Commands::processCommand(String *command, HardwareSerial *interface, App *app) {
  // Parse the string to check if it's a valid command
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(*command);

  if (root.success()) {
    if (root["type"].is<int>() && root["data"].is<JsonObject>()) {
      Commands::RequestType rqt = root["type"].as<int>();

      // Create a dynamic buffer and populate redundant values
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.createObject();
      
      root["type"] = (int)rqt;
      JsonObject& data = root.createNestedObject("data");

      switch (rqt) {
        case Commands::DATA_GLOBAL_RELOAD: {
          JsonObject& data_stats = data.createNestedObject("stats");

          data_stats["weight"] = RawJson(app->getStats()->getStat(Stats::STAT_WEIGHT));
          data_stats["food"] = RawJson(app->getStats()->getStat(Stats::STAT_FOOD));
          data_stats["arrival"] = RawJson(app->getStats()->getStat(Stats::STAT_ARRIVAL));
          
          data["dog"] = RawJson(app->getConfigDog()->generateConfig());
          data["schedule"] = RawJson(app->getConfigSchedule()->generateConfig());

          break;
        }
        case Commands::DATA_STATS_WEIGHT: {
          data["values"] = RawJson(app->getStats()->getStat(Stats::STAT_WEIGHT));
          break;
        }
        case Commands::DATA_STATS_REMAINING_FOOD: {
          data["values"] = RawJson(app->getStats()->getStat(Stats::STAT_FOOD));
          break;
        }
        case Commands::DATA_STATS_DOG_ARRIVAL: {
          data["values"] = RawJson(app->getStats()->getStat(Stats::STAT_ARRIVAL));
          break;
        }
        case Commands::DATA_SCHEDULE_ADD: {
          break;
        }
        case Commands::DATA_SCHEDULE_REMOVE: {
          break;
        }
        case Commands::DATA_SCHEDULE_ENABLE: {
          break;
        }
        case Commands::DATA_SCHEDULE_DATE: {
          break;
        }
        case Commands::DATA_SCHEDULE_RATIO: {
          break;
        }
        case Commands::DATA_DOG_NAME: {
          break;
        }
        case Commands::DATA_DOG_WEIGHT: {
          break;
        }
        case Commands::DATA_DOG_REGULATION_ENABLE: {
          break;
        }
        case Commands::DATA_DOG_REGULATION_VALUE: {
          break;
        }
        default: {
          // Unknown command
          if (rqt < Commands::DATA_GLOBAL_RELOAD || rqt > Commands::DATA_DOG_END)
            Commands::sendError(&Serial, -3);
            
          break;
        }
      }

      root.printTo(Serial);
      Serial.println();
      
      return 1;
    } else {
      sendError(interface, -2);
    }
  } else {
    sendError(interface, -1);
  }

  return 0;
}

void Commands::sendError(HardwareSerial *interface, int err) {
  // Helper func to send an error to the provided serial interface
  interface->println("{\"error\": " + String(err) + "}");
}
