#include "Commands.h"

int Commands::processCommand(String *command, HardwareSerial *interface, App *app) {
  // Parse the string to check if it's a valid command
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, *command);

  if (!error) {
    if (doc["type"].is<int>() && doc["data"].is<JsonObject>()) {
      Commands::RequestType rqt = doc["type"].as<int>();
      JsonObject inData = doc["data"].as<JsonObject>();

      // Populate common values accross request types
      DynamicJsonDocument response(512);
      
      response["type"] = (int)rqt;
      JsonObject data = inData;
      response["data"] = data;

      switch (rqt) {
        case Commands::DATA_GLOBAL_RELOAD: {
          JsonObject data_stats = data.createNestedObject("stats");

          data_stats["weight"] = serialized(app->getStats()->getStat(Stats::STAT_WEIGHT));
          data_stats["food"] = serialized(app->getStats()->getStat(Stats::STAT_FOOD));
          data_stats["arrival"] = serialized(app->getStats()->getStat(Stats::STAT_ARRIVAL));
          
          data["dog"] = serialized(app->getConfigDog()->generateConfig());
          data["schedule"] = serialized(app->getConfigSchedule()->generateConfig());

          break;
        }
        case Commands::DATA_STATS_WEIGHT: {
          data["values"] = serialized(app->getStats()->getStat(Stats::STAT_WEIGHT));
          break;
        }
        case Commands::DATA_STATS_REMAINING_FOOD: {
          data["values"] = serialized(app->getStats()->getStat(Stats::STAT_FOOD));
          break;
        }
        case Commands::DATA_STATS_DOG_ARRIVAL: {
          data["values"] = serialized(app->getStats()->getStat(Stats::STAT_ARRIVAL));
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

          JsonObject value = inData["value"].as<JsonObject>();
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
        case Commands::ESP_UPDATE_TIME: {
          if (data["year"].is<int>() && data["month"].is<int>() && data["date"].is<int>() && data["dow"].is<int>() && data["hour"].is<int>() && data["minute"].is<int>() && data["second"].is<int>()) {
            app->getRTC()->setTime(data["year"].as<int>(), data["month"].as<int>(), data["date"].as<int>(), data["dow"].as<int>(), data["hour"].as<int>(), data["minute"].as<int>(), data["second"].as<int>());
          }
          
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

      serializeJson(doc, *interface);
      interface->println();
      
      return 1;
    } else {
      sendError(interface, -2);
    }
  } else {
    sendError(interface, -1);
  }

  return 0;
}

void Commands::sendError(Print *interface, int err) {
  // Helper func to send an error to the provided serial interface
  interface->println("{\"error\": " + String(err) + "}");
}
