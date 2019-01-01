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

String Stats::generateStats() {
  // Create a buffer and populate it
  const int capacity = JSON_OBJECT_SIZE(3) + 3 * JSON_ARRAY_SIZE(30);
  StaticJsonBuffer<capacity> buffer;
  JsonObject& object = buffer.createObject();

  object.createNestedArray("weight").copyFrom(m_weight);
  object.createNestedArray("arrival").copyFrom(m_arrival);
  object.createNestedArray("food").copyFrom(m_food);

  String ret;
  object.printTo(ret);
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
