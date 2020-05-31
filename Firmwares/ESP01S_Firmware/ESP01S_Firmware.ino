#include "secrets.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
ESP8266WiFiMulti WiFiMulti;

/*
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
boolean newDataToSend = false;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  // wait for WiFi connection

  if ( newDataToSend) {
    const size_t capacity = 4 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5) + 130;
    DynamicJsonDocument doc(capacity);
    doc["size"] = 4;

    JsonObject data1 = doc.createNestedObject("data1");
    data1["type"] = "moisure1";
    data1["value"] = 1234;

    JsonObject data2 = doc.createNestedObject("data2");
    data2["type"] = "level1";
    data2["value"] = 1234;

    JsonObject data3 = doc.createNestedObject("data3");
    data3["type"] = "bomb1";
    data3["value"] = "on";

    JsonObject data4 = doc.createNestedObject("data4");
    data4["type"] = "light1";
    data4["value"] = "off";

    String output;
    serializeJson(doc, output);
    serializeJsonPretty(doc, Serial);
    Serial.println("Serial to send: ");
    Serial.println(output);
    makeHTTPS_POST(output);
  }

  String content = "";
  char character;
  if (Serial.available() ) {

    while (Serial.available()) {
      character = Serial.read();
      content.concat(character);
    }
    delay(1000);
    makeHTTPS_POST(content);
    delay(1000);
  }


  //Serial.println("Wait 1s before next round...");
  delay(1000);
}


int makeHTTPS_POST(String doc) {
  int toReturn = 0;
  if ((WiFiMulti.run() == WL_CONNECTED) ) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    //Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, AWS_API_ENDPOINT)) {  // HTTPS

      https.addHeader("Content-Type", "application/json");
      //Serial.print("[HTTPS] POST...\n");

      // start connection and send HTTP header
      int httpCode = https.POST(doc);
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
