#include "Stats.h"

void Stats::loadStats(JsonObject *stats) {
  // Load all the config
  JsonObject& statsRef = *stats;

  if (statsRef["weight"].is<JsonArray>())
    statsRef["weight"].as<JsonArray>().copyTo(m_weight);

  if (statsRef["arrival"].is<JsonArray>())
    statsRef["arrival"].as<JsonArray>().copyTo(m_arrival);

  if (statsRef["food"].is<JsonArray>())
    statsRef["food"].as<JsonArray>().copyTo(m_food);
}

String Stats::getStat(StatType type) {
  // Create an array to store the 30 days long data
  const int capacity = JSON_ARRAY_SIZE(30);
  StaticJsonBuffer<capacity> buffer;
  JsonArray& jsArray = buffer.createArray();

  // Copy the right array dependign on the wanted stat
  switch (type) {
    case STAT_WEIGHT: {
      jsArray.copyFrom(m_weight);
      break;
    }
    case STAT_ARRIVAL: {
      jsArray.copyFrom(m_arrival);
      break;
    }
    case STAT_FOOD: {
      jsArray.copyFrom(m_food);
      break;
    }
  }

  String ret;
  jsArray.printTo(ret);
  return ret;
}

float Stats::getWeightAt(size_t index) {
  return m_weight[index];
}

unsigned int Stats::getArrivalAt(size_t index) {
  return m_arrival[index];
}

float Stats::getFoodAt(size_t index) {
  return m_food[index];
}
