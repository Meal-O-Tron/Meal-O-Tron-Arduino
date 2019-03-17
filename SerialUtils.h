#ifndef SERIALUTILS_H
#define SERIALUTILS_H

#include "Arduino.h"

class SerialUtils {
  public:
    SerialUtils(HardwareSerial *serialPort);

    void update();
    
    int available();
    
    void begin(long baudrate);
    void end();
    bool isUp();

    String receive();

  private:
    long m_communicationSpeed;
    bool m_isUp;

    String m_actualMessage;
    int m_isAvailable;

    HardwareSerial *m_serialPort;
};

#endif // SERIALUTILS_H
