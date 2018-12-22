#include "Commands.h"
#include "SerialUtils.h"

SerialUtils serial(&Serial);

void setup() {
  serial.begin(115200);
}

void loop() {
  serial.update();

  if (serial.available()) {
    String received = serial.receive();
    Commands::processCommand(&received, &Serial);
  }
}
