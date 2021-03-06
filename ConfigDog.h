#ifndef CONFIGDOG_H
#define CONFIGDOG_H

#include "Arduino.h"
#include <ArduinoJson.h>

class ConfigDog {
  public:
    ConfigDog();

    void loadConfig(JsonDocument *config);
    String generateConfig();

    String getName();
    void setName(String name);

    float getWeight();
    void setWeight(float weight);

    boolean isRegulationEnabled();
    void setRegulationEnabled(boolean enabled);

    unsigned int getRegulationValue();
    void setRegulationValue(unsigned int value);

  private:
    String m_name;
    float m_weight;
    boolean m_weightRegulationEnabled;
    unsigned int m_weightRegulationValue;
};

#endif // CONFIGDOG_H
