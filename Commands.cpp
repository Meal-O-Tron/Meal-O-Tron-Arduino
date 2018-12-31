#include "Commands.h"

int Commands::processCommand(String *command, HardwareSerial *interface, JsonObject *json) {
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(*command);

  if (root.success()) {
    if (root["type"].is<int>() && root["data"].is<JsonObject>()) {
      json = &root;
      return 1;
    } else {
      sendError(interface, -2);
    }
  } else {
    sendError(interface, -1);
  }

  return 0;
}

void Commands::sendError(HardwareSerial *interface, int err) {
  interface->println("{\"error\": " + String(err) + "}");
}
