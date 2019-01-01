#include "ConfigDog.h"

ConfigDog::ConfigDog()
: m_weight(0)
, m_weightRegulationEnabled(false)
, m_weightRegulationValue(0)
{
  
}

void ConfigDog::loadConfig(JsonObject *config) {
  // Load all the config
  JsonObject& configRef = *config;

  if (configRef["name"].is<String>())
    m_name = configRef["name"].as<String>();

  if (configRef["weight"].is<float>())
    m_weight = configRef["weight"].as<float>();

  if (configRef["weight_reg"].is<boolean>())
    m_weightRegulationEnabled = configRef["weight_reg"].as<boolean>();

  if (configRef["weight_reg_value"].is<unsigned int>())
    m_weightRegulationValue = configRef["weight_reg_value"].as<unsigned int>();
}

String ConfigDog::generateConfig() {
  // Create a buffer and populate it
  const int capacity = JSON_OBJECT_SIZE(5);
  StaticJsonBuffer<capacity> buffer;
  JsonObject& object = buffer.createObject();

  object["name"] = m_name;
  object["weight"] = m_weight;
  object["weight_reg"] = m_weightRegulationEnabled;
  object["weight_reg_value"] = m_weightRegulationValue;

  String ret;
  object.printTo(ret);
  return ret;
}

String ConfigDog::getName() {
  return m_name;
}

void ConfigDog::setName(String name) {
  m_name = name;
}

float ConfigDog::getWeight() {
  return m_weight;
}

void ConfigDog::setWeight(float weight) {
  m_weight = weight;
}

boolean ConfigDog::isWeightRegulationEnabled() {
  return m_weightRegulationEnabled;
}

void ConfigDog::setWeightRegulationEnabled(boolean enabled) {
  m_weightRegulationEnabled = enabled;
}

unsigned int ConfigDog::getRegulationValue() {
  return m_weightRegulationValue;
}

void ConfigDog::setRegulationValue(unsigned int value) {
  m_weightRegulationValue = value;
}
