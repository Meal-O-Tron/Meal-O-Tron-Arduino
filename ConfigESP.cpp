#include "ConfigESP.h"

ConfigESP::ConfigESP()
: m_initialized(false)
{
  
}

void ConfigESP::loadConfig(JsonDocument *config) {
  // Load all the config
  JsonDocument configRef = *config;

  if (configRef["ssid"].is<String>())
    m_SSID = configRef["ssid"].as<String>();

  if (configRef["password"].is<String>())
    m_password = configRef["password"].as<String>();

  if (configRef["init"].is<boolean>())
    m_initialized = configRef["init"].as<boolean>();
}

String ConfigESP::generateConfig() {
  // Create a buffer and populate it
  const int capacity = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<capacity> doc;

  doc["ssid"] = m_SSID.c_str();
  doc["password"] = m_password.c_str();
  doc["init"] = m_initialized;

  String ret;
  serializeJson(doc, ret);
  return ret;
}

String ConfigESP::getSSID() {
  return m_SSID;
}

void ConfigESP::setSSID(String SSID) {
  m_SSID = SSID;
}

String ConfigESP::getPassword() {
  return m_password;
}

void ConfigESP::setPassword(String password) {
  m_password = password;
}

boolean ConfigESP::isInitialized() {
  return m_initialized;
}

void ConfigESP::setInitialized(boolean initialized) {
  m_initialized = initialized;
}
