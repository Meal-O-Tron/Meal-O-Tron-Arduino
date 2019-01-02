#include "Commands.h"

int Commands::processCommand(String *command, HardwareSerial *interface, App *app) {
  // Parse the string to check if it's a valid command
  DynamicJsonBuffer jsonBuffer(512);
  JsonObject& root = jsonBuffer.parseObject(*command);

  if (root.success()) {
    if (root["type"].is<int>() && root["data"].is<JsonObject>()) {
      Commands::RequestType rqt = root["type"].as<int>();
      JsonObject& inData = root["data"].as<JsonObject>();

      // Populate common values accross request types
      JsonObject& root = jsonBuffer.createObject();
      
      root["type"] = (int)rqt;
      JsonObject& data = clone(jsonBuffer, inData);
      root["data"] = data;

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
          LinkedList<Schedule*> *appConfigSchedule = app->getConfigSchedule()->getScheduleList();
          if (!inData["hour"].is<int>() || !inData["minute"].is<int>()) {
            sendError(interface, -4);
            return 0;
          }

          boolean enabled = true;
          if (inData["enabled"].is<boolean>())
            enabled = inData["enabled"].as<boolean>();

          unsigned int id = appConfigSchedule->size();
          if (inData["id"].is<unsigned int>())
            id = inData["id"].as<unsigned int>();

          unsigned int ratio = 0;
          if (inData["ratio"].is<unsigned int>())
            ratio = inData["ratio"].as<unsigned int>();

          unsigned int hour = inData["hour"].as<int>();
          unsigned int minute = inData["minute"].as<int>();

          Schedule *schedule = new Schedule(hour, minute, ratio, enabled);
          appConfigSchedule->add(id, schedule);

          data["hour"] = hour;
          data["minute"] = minute;
          data["id"] = id;
          data["ratio"] = ratio;
          data["enabled"] = enabled;
          
          break;
        }
        case Commands::DATA_SCHEDULE_REMOVE: {
          if (!inData["id"].is<int>()) {
            sendError(interface, -4);
            return 0;
          }
          
          Schedule *deletedSchedule = app->getConfigSchedule()->getScheduleList()->remove(inData["id"].as<int>());
          delete deletedSchedule;
          deletedSchedule = NULL;
            
          break;
        }
        case Commands::DATA_SCHEDULE_ENABLE: {
          if (!inData["id"].is<int>() || !inData["value"].is<boolean>()) {
            sendError(interface, -4);
            return 0;
          }

          Schedule *schedule = app->getConfigSchedule()->getScheduleList()->get(inData["id"].as<int>());
          schedule->setEnabled(inData["value"].as<boolean>());
          
          break;
        }
        case Commands::DATA_SCHEDULE_DATE: {
          if (!inData["id"].is<int>() || !inData["value"].is<JsonObject>()) {
            sendError(interface, -4);
            return 0;
          }

          JsonObject& value = inData["value"].as<JsonObject>();
          if (!value["hour"].is<unsigned int>() || !value["minute"].is<unsigned int>()) {
            sendError(interface, -4);
            return 0;
          }

          Schedule *schedule = app->getConfigSchedule()->getScheduleList()->get(inData["id"].as<int>());
          schedule->setHour(value["hour"].as<unsigned int>());
          schedule->setMinute(value["minute"].as<unsigned int>());
          
          break;
        }
        case Commands::DATA_SCHEDULE_RATIO: {
          if (!inData["id"].is<int>() || !inData["value"].is<int>()) {
            sendError(interface, -4);
            return 0;
          }

          Schedule *schedule = app->getConfigSchedule()->getScheduleList()->get(inData["id"].as<int>());
          schedule->setRatio(inData["value"].as<int>());
          
          break;
        }
        case Commands::DATA_DOG_NAME: {
          if (!inData["value"].is<String>()) {
            sendError(interface, -4);
            return 0;
          }

          app->getConfigDog()->setName(inData["value"].as<String>());
          
          break;
        }
        case Commands::DATA_DOG_WEIGHT: {
          data["value"] = app->getConfigDog()->getWeight();
          
          break;
        }
        case Commands::DATA_DOG_REGULATION_ENABLE: {
          if (!inData["value"].is<boolean>()) {
            sendError(interface, -4);
            return 0;
          }

          app->getConfigDog()->setRegulationEnabled(inData["value"].as<boolean>());
          
          break;
        }
        case Commands::DATA_DOG_REGULATION_VALUE: {
          if (!inData["value"].is<unsigned int>()) {
            sendError(interface, -4);
            return 0;
          }

          app->getConfigDog()->setRegulationValue(inData["value"].as<unsigned int>());
          
          break;
        }
        default: {
          // Unknown command
          if (rqt < Commands::DATA_GLOBAL_RELOAD || rqt > Commands::DATA_DOG_END)
            Commands::sendError(&Serial, -3);
            
          break;
        }
      }

      if (rqt > DATA_SCHEDULE_START && rqt < DATA_SCHEDULE_END)
        app->saveConfigSchedule();
      else if (rqt > DATA_DOG_START && rqt < DATA_DOG_END)
        app->saveConfigDog();

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

JsonVariant Commands::clone(JsonBuffer& jb, JsonVariant prototype) {
  if (prototype.is<JsonObject>()) {
    const JsonObject& protoObj = prototype;
    JsonObject& newObj = jb.createObject();
    for (const auto& kvp : protoObj) {
      newObj[jb.strdup(kvp.key)] = clone(jb, kvp.value);
    }
    return newObj;
  }

  if (prototype.is<JsonArray>()) {
    const JsonArray& protoArr = prototype;
    JsonArray& newArr = jb.createArray();
    for (const auto& elem : protoArr) {
      newArr.add(clone(jb, elem));
    }
    return newArr;
  }

  if (prototype.is<char*>()) {
    return jb.strdup(prototype.as<const char*>());
  }

  return prototype;
}
