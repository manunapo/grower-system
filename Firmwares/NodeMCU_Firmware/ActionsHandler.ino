#include "defines.h"

boolean watering = false;

void setUpActions() {
  digitalWrite(BMBPIN, HIGH);
  pinMode(BMBPIN, OUTPUT);

  digitalWrite(LGTPIN, HIGH);
  pinMode(LGTPIN, OUTPUT);

  digitalWrite(FANPIN, HIGH);
  pinMode(FANPIN, OUTPUT);
}


void startBomb() {
  digitalWrite(BMBPIN, LOW);
  Serial.println("START BOMB");
  watering = true;
}

void stopBomb() {
  digitalWrite(BMBPIN, HIGH);
  Serial.println("STOP BOMB");
  watering = false;
}

boolean isWatering() {
  return watering;
}

void startLight() {
  digitalWrite(LGTPIN, LOW);
  Serial.println("START LIGHT");
}

void stopLight() {
  digitalWrite(LGTPIN, HIGH);
  Serial.println("STOP LIGHT");
}


void startFan() {
  digitalWrite(FANPIN, LOW);
  Serial.println("START FAN");
}

void stopFan() {
  digitalWrite(FANPIN, HIGH);
  Serial.println("STOP FAN");
}

void processActionFromTopic(String json) {
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + 30;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, json);
  const char* light = doc["light"];
  const char* fan = doc["fan"];
  const char* bomb = doc["bomb"];

  String lightS = (String)light;
  String fanS = (String)fan;
  String bombS = (String)bomb;

  if ( lightS.equals("on"))
  {
    Serial.println("Turn On lights");
    startLight();
  }
  if ( fanS.equals("on"))
  {
    Serial.println("Turn On fan");
    startFan();
  }
  if ( bombS.equals("on"))
  {
    Serial.println("Turn On bomb");
    startBomb();
  }
  if ( lightS.equals("off"))
  {
    Serial.println("Turn Off lights");
    stopLight();
  }
  if ( fanS.equals("off"))
  {
    Serial.println("Turn Off fan");
    stopFan();
  }

  // Turn off bomb is not going to be use, because bomb will be automatically turned off
  // after 5 seconds ( or other pre-set time.
  if ( bombS.equals("off"))
  {
    Serial.println("Turn Off bomb ");
  }
}
