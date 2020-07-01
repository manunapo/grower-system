#include "DHT.h"
#include "defines.h"


DHT dht(DHTPIN, DHTTYPE);

void setUpSensors() {
  dht.begin();
  pinMode(DHTPIN, OUTPUT);
}

float readDHTHumidity() {
  return dht.readHumidity();
}

float readDHTTemperature() {
  return dht.readTemperature();
}

String buildJSON() {

  String toRet = "";
  const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);

  JsonObject state = doc.createNestedObject("state");

  JsonObject state_reported = state.createNestedObject("reported");
  state_reported["ah"] = random(100);
  state_reported["gh"] = random(100);
  state_reported["at"] = random(100);
  state_reported["lt"] = "on";
  serializeJson(doc, toRet);

  return toRet;
}
