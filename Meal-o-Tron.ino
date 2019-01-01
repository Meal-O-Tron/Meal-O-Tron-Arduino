#include <SD.h>
#include "Commands.h"
#include "ConfigDog.h"
#include "ConfigSchedule.h"
#include "SerialUtils.h"

SerialUtils serial(&Serial);

ConfigDog configDog;
ConfigSchedule configSchedule;

void setup() {
  // Init serial port 0
  serial.begin(115200);

  // Init SD Card
  if (SD.begin(26)) {
    Serial.println("[OK] SD Card");
    File configFile;

    // Load dog configuration file
    if (SD.exists("dog.cfg")) {
      String savedConfigDog;
      configFile = SD.open("dog.cfg", FILE_READ);

      while (configFile.available()) {
        savedConfigDog += (char)configFile.read();
      }

      configFile.close();
      
      StaticJsonBuffer<256> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(savedConfigDog);
      if (root.success()) {
        configDog.loadConfig(&root);
      }
    }

    if (SD.exists("sched.cfg")) {
      String savedConfigSchedule;
      configFile = SD.open("sched.cfg", FILE_READ);

      while (configFile.available()) {
        savedConfigSchedule += (char)configFile.read();
      }

      configFile.close();
      
      StaticJsonBuffer<256> jsonBuffer;
      JsonArray& root = jsonBuffer.parseArray(savedConfigSchedule);
      if (root.success()) {
        configSchedule.loadConfig(&root);
      }
    }

    Serial.println(configSchedule.generateConfig());
  } else {
    Serial.println("[FAIL] SD Card");
  }
  
  Serial.println("=== MEAL-O-TRON STARTED ===");
}

void loop() {
  // Read serial ports for new data
  serial.update();

  // Data available (ends with \n)
  if (serial.available()) {
    // Store the received string and try to process it
    String received = serial.receive();
    JsonObject *json;

    // Check if the string can be processed
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
          // Unknown command
          if (rqt < Commands::DATA_GLOBAL_RELOAD || rqt > Commands::DATA_DOG_END)
            Commands::sendError(&Serial, -3);
            
          break;
        }
      }
    }
  }
}
