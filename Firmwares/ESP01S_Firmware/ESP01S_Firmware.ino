#include "secrets.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/*
   Receibe from arduino a compressed json
   for less traffic in serial interface
   {
     "t1":  1234,
     "m1":  1234,
     "m2":  1234
  }


   {
     "size" : 4,
      "data1": {
          "type": "moisure1",
          "value": 1234
      },
      "data2": {
          "type": "level1",
          "value": 1234
      },
      "data3": {
          "type": "bomb1",
          "value": "on"
      },
      "data4": {
          "type": "light1",
          "value": "off"
      }
  }
*/
//Capacity for 4 data elements
//const size_t capacity = 4*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5) + 130;
//DynamicJsonDocument doc(capacity);

int HTTPresponse;

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP;

const int sizeArduinoJson = 3;

int totalDelay = 10000;

// Total time to do the action. Default values.
// Definitive values will be loaded from memory
int wateringTime = 5; // Expresed in seconds.
int lightingTime = 7; // Expresed in hours.

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

boolean hadSome = false;

void setup() {

  Serial.begin(9600);
  Serial.flush();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while ( WiFi.status() != WL_CONNECTED ) {
    //Serial.println("WIFI not connected");
    delay ( 500 );
  }
  //Serial.println("--- WIFI  connected");

  // First Set up Memory Handler
  setUpMemoryHandler();
  delay( 500);
  //saveConfigurationParameters(14,14,6,8);
  //saveLastWatering( 12, 12, 12, 6);

  setUpTimeHandler();

  wateringTime = getWateringTime();
  lightingTime = getLightingTime();
}



void loop() {

  updateTime();

  /*
    Serial.println();
    Serial.print(" Config Params: "); Serial.print( getWateringTime());Serial.print(" ");
    Serial.print( getLightingTime()); Serial.print(" "); Serial.print( getHourToWatering());
    Serial.print(" ");Serial.print( getHourToLighting()); Serial.println();

    Serial.print(" Last Watering: "); Serial.println(getLastWateringTime());
  */

  if ( mayStartWatering()) {
    String watTime = (String) wateringTime;
    Serial.println(  "SW-" + watTime);
    delay(1000);
    totalDelay -= 1000;
  }

  if ( mayStartLighting()) {
    String ligTime = (String) lightingTime;
    Serial.println( "SL-" + ligTime);
    delay(1000);
    totalDelay -= 1000;
  }

  String sensorsCompressed = "{\"t1\":0,\"m1\":0,\"m2\":0}";

  if (Serial.available() ) {
    while (Serial.available()) {
      sensorsCompressed = Serial.readStringUntil( '\n' );
      hadSome = true;
    }
    String json =  buildJSON(sensorsCompressed);
    if ( hadSome) {
      makeHTTPS_POST(json);
      hadSome = false;
      delay(1000);
      totalDelay -= 1000;
    }
  }

  delay(totalDelay);
}


String buildJSON(String jsonArduino) {

  const size_t capacityArduino = JSON_OBJECT_SIZE(sizeArduinoJson) + ( sizeArduinoJson * 10);
  DynamicJsonDocument docArduino(capacityArduino);
  DeserializationError err = deserializeJson(docArduino, jsonArduino);

  if (err) {
    hadSome = false;
    return "error";
  }

  int sizeAWSJson = sizeArduinoJson + 2; //2 = bomb + light;
  const size_t capacityAWS = sizeAWSJson * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(sizeAWSJson + 1) + (sizeAWSJson * 10);
  DynamicJsonDocument docAWS(capacityAWS);

  docAWS["size"] = sizeAWSJson;

  JsonObject data1 = docAWS.createNestedObject("data1");
  data1["type"] = "air_temperature";
  data1["value"] = docArduino["t1"];

  JsonObject data2 = docAWS.createNestedObject("data2");
  data2["type"] = "air_moisure";
  data2["value"] = docArduino["m1"];

  JsonObject data3 = docAWS.createNestedObject("data3");
  data3["type"] = "ground_moisure";
  data3["value"] = docArduino["m2"];

  JsonObject data4 = docAWS.createNestedObject("data4");
  data4["type"] = "bomb";
  data4["value"] = getLastWateringTime() + " a:" + amountOfWaterLastWatering();

  JsonObject data5 = docAWS.createNestedObject("data5");
  data5["type"] = "light";
  data5["value"] = getLastLightingTime() + " a:" + amountOfLightLastLighting();;

  String output;
  serializeJson(docAWS, output);

  return output;
}

int amountOfWaterLastWatering() {
  // In  2 minutes (120 seconds) the machine waters 100cc of water.
  return wateringTime * 100 / 120;
}

int amountOfLightLastLighting() {
  return lightingTime;
}

int makeHTTPS_POST(String json) {
  int toReturn = 0;
  if ((WiFi.status() == WL_CONNECTED) ) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    //Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, AWS_API_ENDPOINT)) {  // HTTPS

      https.addHeader("Content-Type", "application/json");
      //Serial.print("[HTTPS] POST...\n");

      // start connection and send HTTP header
      int httpCode = https.POST(json);
      //int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      } else {
        //Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      String payload = https.getString();
      Serial.println(payload);
      https.end();
      toReturn = httpCode;
    } else {
      //Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  return toReturn;
}
