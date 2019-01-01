#ifndef STATS_H
#define STATS_H

#include "Arduino.h"
#include <ArduinoJson.h>

class Stats {
  public:
    void loadStats(JsonObject *stats);
    String generateStats();

    float getWeightAt(size_t index);
    unsigned int getArrivalAt(size_t index);
    float getFoodAt(size_t index);

  private:
    float m_weight[30];
    unsigned int m_arrival[30];
    float m_food[30];
};

#endif
