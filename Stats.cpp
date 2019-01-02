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

  // Check how many days are worth copying (i.e. not blank)
  int daysToCopy = 30;
  for (int i = 29; i >= 0; i--) {
    if (m_weight[i] != 0 || m_arrival[i] != 0 || m_food[i] != 0) {
      daysToCopy = i + 1;
      break;
    }
  }

  // Copy the right array depending on the wanted stat
  switch (type) {
    case STAT_WEIGHT: {
      jsArray.copyFrom(m_weight, daysToCopy);
      break;
    }
    case STAT_ARRIVAL: {
      jsArray.copyFrom(m_arrival, daysToCopy);
      break;
    }
    case STAT_FOOD: {
      jsArray.copyFrom(m_food, daysToCopy);
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
