#include "Stats.h"

void Stats::loadStats(JsonDocument *stats) {
  // Load all the config
  JsonDocument statsRef = *stats;

  if (statsRef["weight"].is<JsonArray>())
    copyArray(statsRef["weight"].as<JsonArray>(), m_weight);

  if (statsRef["arrival"].is<JsonArray>())
    copyArray(statsRef["arrival"].as<JsonArray>(), m_arrival);

  if (statsRef["food"].is<JsonArray>())
    copyArray(statsRef["food"].as<JsonArray>(), m_food);
}

String Stats::getStat(StatType type) {
  // Create an array to store the 30 days long data
  const int capacity = JSON_ARRAY_SIZE(30);
  StaticJsonDocument<capacity> doc;
  JsonArray jsArray = doc.to<JsonArray>();

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
      copyArray(m_weight, daysToCopy, jsArray);
      break;
    }
    case STAT_ARRIVAL: {
      copyArray(m_arrival, daysToCopy, jsArray);
      break;
    }
    case STAT_FOOD: {
      copyArray(m_food, daysToCopy, jsArray);
      break;
    }
  }

  String ret;
  serializeJson(jsArray, ret);
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
