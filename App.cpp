#include <SD.h>
#include "App.h"
#include "Commands.h"

App::App()
: m_serial(&Serial)
{
  
}

void App::setup() {
  // Init serial port 0
  m_serial.begin(115200);

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
        m_configDog.loadConfig(&root);
      }
    }

    // Load the schedule configuration file
    if (SD.exists("sched.cfg")) {
      String savedConfigSchedule;
      configFile = SD.open("sched.cfg", FILE_READ);

      while (configFile.available()) {
        savedConfigSchedule += (char)configFile.read();
      }

      configFile.close();
      
      StaticJsonBuffer<512> jsonBuffer;
      JsonArray& root = jsonBuffer.parseArray(savedConfigSchedule);
      if (root.success()) {
        m_configSchedule.loadConfig(&root);
      }
    }

    // Load previously saved statistics
    if (SD.exists("stats.cfg")) {
      String savedStats;
      configFile = SD.open("stats.cfg", FILE_READ);

      while (configFile.available()) {
        savedStats += (char)configFile.read();
      }

      configFile.close();

      const int capacity = JSON_OBJECT_SIZE(3) + 3 * JSON_ARRAY_SIZE(30);
      StaticJsonBuffer<capacity> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(savedStats);
      if (root.success()) {
        m_stats.loadStats(&root);
      }
    }
  } else {
    Serial.println("[FAIL] SD Card");
  }
  
  Serial.println("=== MEAL-O-TRON STARTED ===");
}

void App::loop() {
  // Read serial ports for new data
  m_serial.update();

  // Data available (ends with \n)
  if (m_serial.available()) {
    // Store the received string and try to process it
    String received = m_serial.receive();

    // Check if the string can be processed
    if (Commands::processCommand(&received, &Serial, this)) {
      
    }
  }
}

ConfigDog* App::getConfigDog() {
  return &m_configDog;
}

ConfigSchedule* App::getConfigSchedule() {
  return &m_configSchedule;
}

Stats* App::getStats() {
  return &m_stats;
}

void App::saveConfigDog() {
  // Clear the file and save the configuration
  SD.remove("dog.cfg");
  
  File configFile = SD.open("dog.cfg", FILE_WRITE);
  configFile.print(m_configDog.generateConfig());
  configFile.close();
}

void App::saveConfigSchedule() {
  // Clear the file and save the configuration
  SD.remove("sched.cfg");
  
  File configFile = SD.open("sched.cfg", FILE_WRITE);
  configFile.print(m_configSchedule.generateConfig());
  configFile.close();
}
