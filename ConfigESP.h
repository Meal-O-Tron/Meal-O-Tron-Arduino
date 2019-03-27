#ifndef CONFIGESP_H
#define CONFIGESP_H

#include "Arduino.h"
#include <ArduinoJson.h>

class ConfigESP {
  public:
    ConfigESP();

    void loadConfig(JsonDocument *config);
    String generateConfig();

    String getSSID();
    void setSSID(String SSID);

    String getPassword();
    void setPassword(String password);

    boolean isInitialized();
    void setInitialized(boolean initialized);

  private:
    String m_SSID;
    String m_password;
    boolean m_initialized;
};

#endif // CONFIGESP_H
