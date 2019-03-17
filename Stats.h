#ifndef STATS_H
#define STATS_H

#include "Arduino.h"
#include <ArduinoJson.h>

class Stats {
  public:
    enum StatType {
      STAT_WEIGHT,
      STAT_ARRIVAL,
      STAT_FOOD,
    };
    
    void loadStats(JsonDocument *stats);
    String getStat(StatType type);

    float getWeightAt(size_t index);
    unsigned int getArrivalAt(size_t index);
    float getFoodAt(size_t index);

  private:
    float m_weight[30];
    unsigned int m_arrival[30];
    float m_food[30];
};

#endif // STATS_H
