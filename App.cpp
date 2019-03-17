#include <SD.h>
#include "App.h"
#include "Commands.h"

App::App()
: m_serial(&Serial1)
{
  
}

void App::setup() {
  // Init serial port 0 (USB)
  Serial.begin(9600);

  // Init RTC
  m_rtc.begin();

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
      
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, savedConfigDog);
      if (!error) {
        m_configDog.loadConfig(&doc);
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
      
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, savedConfigSchedule);
      if (!error) {
        m_configSchedule.loadConfig(&doc);
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
      StaticJsonDocument<capacity> doc;
      DeserializationError error = deserializeJson(doc, savedStats);
      if (!error) {
        m_stats.loadStats(&doc);
      }
    }
  } else {
    Serial.println("[FAIL] SD Card");
  }

  // To read ESP state
  pinMode(2, INPUT_PULLUP);
  
  Serial.println("=== MEAL-O-TRON STARTED ===");
}

void App::loop() {
  checkESPState();
  
  // Update rtc status and check for scheduled event
  m_rtc.update();

  if (m_rtc.getCurrentDate().minute() != m_rtc.getPreviousDate().minute())
    onMinuteChanged();
  
  // Read serial ports for new data
  m_serial.update();

  // Data available (ends with \n)
  if (m_serial.available()) {
    // Store the received string and try to process it
    String received = m_serial.receive();
    if (received.length() > 0) {
      Serial.println(received);

      // Process the string
      Commands::processCommand(&received, &Serial1, this);
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

RTC* App::getRTC() {
  return &m_rtc;
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

String App::getConfigESP() {
  if (SD.exists("esp.cfg")) {
    String savedConfigESP;
    File configFile = SD.open("esp.cfg", FILE_READ);
    
    while (configFile.available()) {
      char actualChar = (char)configFile.read();
      if (actualChar == '\n')
        break;
        
      savedConfigESP += actualChar;
    }

    configFile.close();
    return savedConfigESP;
  } else {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    String configESP;

    doc["init"] = false;

    serializeJson(doc, configESP);
    return configESP;
  }
}

void App::onMinuteChanged() {
  LinkedList<Schedule*> *scheduleList = m_configSchedule.getScheduleList();
  DateTime currentDate = m_rtc.getCurrentDate();
  
  for (int i = 0; i < scheduleList->size(); i++) {
    Schedule *currentSchedule = scheduleList->get(i);
    if (currentSchedule->isEnabled() && currentSchedule->getHour() == currentDate.hour() && currentSchedule->getMinute() == currentDate.minute()) {
      Serial.println("okay");
    }
  }
}

void App::checkESPState() {
  if (!digitalRead(2) && !m_serial.isUp()) {
    // ESP is up, send informations
    m_serial.begin(115200);

    StaticJsonDocument<512> doc;

    doc["type"] = 100;
    doc["data"] = serialized(getConfigESP());

    serializeJson(doc, Serial1);
    Serial1.println();
  } else if (digitalRead(2) && m_serial.isUp()) {
    // ESP is down
    m_serial.end();
  }
}
