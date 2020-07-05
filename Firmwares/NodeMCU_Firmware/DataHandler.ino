#include "DHT.h"
#include "defines.h"

DHT dht(DHTPIN, DHTTYPE);

const int sensorPin = A0;

void setUpSensors() { 
  pinMode(DHTPIN, OUTPUT); 
  digitalWrite(DHTPIN, LOW);
  digitalWrite(DHTPIN, HIGH);
  dht.begin();
}

float readDHTHumidity() {
  return dht.readHumidity();
}

float readDHTTemperature() {
  return dht.readTemperature();
}

int readGroundHumidity(){
  return analogRead(GRDPIN);
}

String buildJSON() {

  String toRet = "";
  const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);

  JsonObject state = doc.createNestedObject("state");

  JsonObject state_reported = state.createNestedObject("reported");
  state_reported["ah"] = readDHTHumidity();
  state_reported["gh"] = readGroundHumidity();
  state_reported["at"] = readDHTTemperature();
  state_reported["lt"] = "on";
  serializeJson(doc, toRet);

  return toRet;
}
