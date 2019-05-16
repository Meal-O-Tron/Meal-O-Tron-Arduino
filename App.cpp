#include <SD.h>
#include "App.h"
#include "Commands.h"

App::App()
: m_serial(&Serial1)
, m_oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
, m_displayReady(false)
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

    // Load ESP configuration
    if (SD.exists("esp.cfg")) {
      String savedESP;
      configFile = SD.open("esp.cfg", FILE_READ);

      while (configFile.available()) {
        savedESP += (char)configFile.read();
      }

      configFile.close();
      
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, savedESP);
      if (!error) {
        m_configESP.loadConfig(&doc);
      }
    }
  } else {
    Serial.println("[FAIL] SD Card");
  }

  // Init OLED
  if (m_oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[OK] OLED Display");

    m_displayReady = true;
  } else {
    Serial.println("[FAIL] OLED Display");
  }

  // To read ESP state
  pinMode(2, INPUT_PULLUP);

  // Food distribution pins
  pinMode(8, OUTPUT);
  
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

  // Stop the distributor if running and food has been distributed
  if (m_distributing && analogRead(A0) >= 300) {
    digitalWrite(8, LOW);
    m_distributing = false;

    Serial.println("Stopping distribution");
  }

  if (m_displayReady) {
    m_oledDisplay.clearDisplay();

    m_oledDisplay.setTextSize(2);
    m_oledDisplay.setTextColor(WHITE);
    m_oledDisplay.setCursor(0, 0);
    m_oledDisplay.println(m_configDog.getName());
    m_oledDisplay.println(String(m_configDog.getWeight()) + "kg");

    DateTime currentTime = m_rtc.getCurrentDate();

    m_oledDisplay.setTextSize(1);
    m_oledDisplay.setCursor(0, 55);
    m_oledDisplay.print(String(currentTime.hour()) + ":" + String(currentTime.minute()));

    if (m_distributing) {
      m_oledDisplay.println(" - Distributing");
    } else {
      m_oledDisplay.println();
    }

    m_oledDisplay.display();
  }

  // Data available (ends with \n)
  if (m_serial.available()) {
    // Store the received string and try to process it
    String received = m_serial.receive();
    if (received.length() > 0) {
      Serial.print("Received command: ");
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

ConfigESP* App::getConfigESP() {
  return &m_configESP;
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

void App::saveConfigESP() {
  // Clear the file and save the configuration
  SD.remove("esp.cfg");
  
  File configFile = SD.open("esp.cfg", FILE_WRITE);
  configFile.print(m_configESP.generateConfig());
  configFile.close();
}

void App::resetESP() {
  StaticJsonDocument<1024> doc;

  doc["type"] = 103;
  doc.createNestedObject("data");

  serializeJson(doc, Serial1);
  Serial1.println();
}

void App::onMinuteChanged() {
  LinkedList<Schedule*> *scheduleList = m_configSchedule.getScheduleList();
  DateTime currentDate = m_rtc.getCurrentDate();

  Serial.println("Minute has changed, " + String(currentDate.hour()) + ":" + String(currentDate.minute()));
  
  for (int i = 0; i < scheduleList->size(); i++) {
    Schedule *currentSchedule = scheduleList->get(i);
    Serial.println("Checking " + String(currentSchedule->getHour()) + ":" + String(currentSchedule->getMinute()));
    if (currentSchedule->isEnabled() && currentSchedule->getHour() == currentDate.hour() && currentSchedule->getMinute() == currentDate.minute()) {
      digitalWrite(8, HIGH);
      m_distributing = true;

      Serial.println("Starting distribution");
    }
  }
}

void App::checkESPState() {
  if (!digitalRead(2) && !m_serial.isUp()) {
    // ESP is up, send informations
    m_serial.begin(115200);

    StaticJsonDocument<1024> doc;

    doc["type"] = 100;
    doc["data"] = serialized(m_configESP.generateConfig());
    doc["name"] = m_configDog.getName();
    
    JsonObject data = doc["data"].as<JsonObject>();
    data["name"] = m_configDog.getName();

    serializeJson(doc, Serial1);
    Serial1.println();
  } else if (digitalRead(2) && m_serial.isUp()) {
    // ESP is down
    m_serial.end();
  }
}
