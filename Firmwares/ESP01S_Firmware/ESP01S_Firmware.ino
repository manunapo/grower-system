#include "secrets.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/*
 * Receibe from arduino a compressed json
 * for less traffic in serial interface
 * {
     "t1":  1234,
     "m1":  1234,
     "m2":  1234
  }
 * 
 * 
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

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.google.com", 3600, 60000);

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; 
const char* ntpServerName = "time.google.com";

const int sizeArduinoJson = 3;

int totalDelay = 10000;

// Last time the action was done
String lastWateringTime = "";
String lastLightingTime = "";

// Total time to do the action
int wateringTime = 5; // Expresed in seconds. 5 seconds
int lightingTime = 7; // Expresed in hours. 7 hours

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup() {

  Serial.begin(9600);
  Serial.flush();
  // Serial.setDebugOutput(true);


  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("WIFI not connected");
    delay ( 500 );
  }  
  Serial.println("--- WIFI  connected");
  timeClient.begin();
}



void loop() {

  timeClient.update();
  String theTime = timeClient.getFormattedTime();

  if( mayStartWatering()){
    String command = "SW-" + wateringTime;
    Serial.println(command);
    lastWateringTime = theTime;
    delay(1000);
    totalDelay -= 1000;
  }

  if( mayStartLighting()){
    String command = "SL-" + lightingTime;
    Serial.println(command);
    lastLightingTime = theTime;
    delay(1000);
    totalDelay -= 1000;
  }
 
  String sensorsCompressed = "{\"t1\":0,\"m1\":0,\"m2\":0}";
  
  if (Serial.available() ) {

    while (Serial.available()) {
      sensorsCompressed = Serial.readStringUntil( '\n' );
    }
    
    makeHTTPS_POST( buildJSON(sensorsCompressed));  
    delay(1000);
    totalDelay -= 1000;
  }
  //String json = "{\"t1\":1234,\"m1\":1234,\"m2\":1234}";
  //Serial.println("JSON Build: ");
  //Serial.println(buildJSON(json));
  
  delay(totalDelay);
}


boolean mayStartWatering(){
  return true;
}

boolean mayStartLighting(){
  return true;
}
String buildJSON(String jsonArduino){
  
  const size_t capacityArduino = JSON_OBJECT_SIZE(sizeArduinoJson) + ( sizeArduinoJson * 10);
  DynamicJsonDocument docArduino(capacityArduino);
  deserializeJson(docArduino, jsonArduino);

  int sizeAWSJson = sizeArduinoJson + 2; //2 = bomb + light;
  const size_t capacityAWS = sizeAWSJson * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(sizeAWSJson +1) + (sizeAWSJson * 10);
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
  data4["value"] = lastWateringTime + "-" + amountOfWaterLastWatering();

  JsonObject data5 = docAWS.createNestedObject("data5");
  data5["type"] = "light";
  data5["value"] = lastLightingTime + "-" + amountOfLightLastLighting();;

  String output;
  serializeJson(docAWS, output);

  return output;
}

int amountOfWaterLastWatering(){
  // In  2 minutes (120 seconds) the machine waters 100cc of water.
  return wateringTime * 100 / 120;
}

int amountOfLightLastLighting(){
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
