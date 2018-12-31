#include "Commands.h"
#include "SerialUtils.h"

SerialUtils serial(&Serial);

void setup() {
  serial.begin(115200);
  Serial.println("=== MEAL-O-TRON STARTED ===");
}

void loop() {
  serial.update();

  if (serial.available()) {
    String received = serial.receive();
    JsonObject *json;
    
    if (Commands::processCommand(&received, &Serial, json)) {
      Commands::RequestType rqt = (*json)["type"].as<int>();

      switch (rqt) {
        case Commands::DATA_GLOBAL_RELOAD: {
          break;
        }
        case Commands::DATA_STATS_WEIGHT: {
          break;
        }
        case Commands::DATA_STATS_REMAINING_FOOD: {
          break;
        }
        case Commands::DATA_STATS_DOG_ARRIVAL: {
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
          if (rqt < Commands::DATA_GLOBAL_RELOAD || rqt > Commands::DATA_DOG_END)
            Commands::sendError(&Serial, -3);
            
          break;
        }
      }
    }
  }
}
