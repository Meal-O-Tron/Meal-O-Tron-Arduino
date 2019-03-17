#include "ConfigSchedule.h"

ConfigSchedule::ConfigSchedule() {

}

void ConfigSchedule::loadConfig(JsonDocument* config) {
  // Load all the config
  JsonArray configRef = config->as<JsonArray>();

  for (int i = 0; i < configRef.size(); i++) {
    if (configRef[i].is<JsonObject>()) {
      JsonObject currentObject = configRef[i].as<JsonObject>();

      if (currentObject["hour"].is<unsigned int>() && currentObject["minute"].is<unsigned int>() && currentObject["ratio"].is<unsigned int>() && currentObject["enabled"].is<boolean>()) {
        Schedule *schedule = new Schedule(currentObject["hour"].as<unsigned int>(), currentObject["minute"].as<unsigned int>(), currentObject["ratio"].as<unsigned int>(), currentObject["enabled"].as<boolean>());
        m_scheduleList.add(schedule);
      }
    }
  }
}

String ConfigSchedule::generateConfig() {
  // Create a buffer and populate it
  DynamicJsonDocument doc(512);
  JsonArray jsArray = doc.to<JsonArray>();

  for (int i = 0; i < m_scheduleList.size(); i++) {
    JsonObject object = jsArray.createNestedObject();
    Schedule *currentSchedule = m_scheduleList.get(i);

    object["hour"] = currentSchedule->getHour();
    object["minute"] = currentSchedule->getMinute();
    object["ratio"] = currentSchedule->getRatio();
    object["enabled"] = currentSchedule->isEnabled();
  }

  String ret;
  serializeJson(doc, ret);
  return ret;
}

LinkedList<Schedule*>* ConfigSchedule::getScheduleList() {
  return &m_scheduleList;
}
