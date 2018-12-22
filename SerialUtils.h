#ifndef SERIALUTILS_H
#define SERIALUTILS_H

#include "Arduino.h"

class SerialUtils {
  public:
    SerialUtils(HardwareSerial *serialPort);

    void update();
    
    int available();
    void begin(long baudrate);

    String receive();

  private:
    long m_communicationSpeed;

    String m_actualMessage;
    int m_isAvailable;

    HardwareSerial *m_serialPort;
};

#endif
