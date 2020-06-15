#include <SoftwareSerial.h>
#include <ArduinoJson.h>

/*
  1 second = 1000 milliseconds
  1 minute = 60 seconds
  1 hour = 60 minutes
  12 hours = 12 * 60 * 60 * 1000 = 43,200,000
  15 minutes = 15 * 60 * 1000 = 900,000
*/
#define SAMPLING 900000UL
unsigned long startTimeSampling;
unsigned long startTimeWatering;

boolean isWatering = false;
unsigned long wateringSeconds = 1000;

SoftwareSerial ESP01S(7, 8); // TX, RX

void setup() {
  Serial.begin(9600);

  ESP01S.begin(9600);

  ESP01S.flush();

  startTimeSampling = millis();
  startTimeWatering = startTimeSampling;

  String jsonToSend = senseAndBuildJSON();
  Serial.print("To send: "); Serial.println(jsonToSend);
  ESP01S.println(jsonToSend);

  setUpBomb();
}

String content = "";
char character;

void loop() {

  unsigned long elapsedTime = millis();

  if ( elapsedTime - startTimeSampling > SAMPLING) {
    String jsonToSend = senseAndBuildJSON();
    ESP01S.println(jsonToSend);
    startTimeSampling = elapsedTime;
    Serial.println("DATA SENT");
  }

  if( isWatering){
    if ( elapsedTime - startTimeWatering > wateringSeconds) {
      stopBombing();
      isWatering = false;
      startTimeWatering = elapsedTime;
    }
  }

  if (ESP01S.available()) {
    while (ESP01S.available()) {
      content = ESP01S.readStringUntil( '\n' );
      if ( content.charAt(1) == 'W') {
        unsigned int seconds = String(content.charAt(3)).toInt();
        wateringSeconds = wateringSeconds * seconds;
        startBombing();
        isWatering = true;
        startTimeWatering = elapsedTime;
      }
      if ( content.charAt(1) == 'L') {
        //Serial.println("LIGHT!");
      }
    }

  }

  Serial.println("...");
  delay(500);
}

String senseAndBuildJSON() {
  const size_t capacity = JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);

  doc["t1"] = random(1000);
  doc["m1"] = random(1000);
  doc["m2"] = random(1000);

  String output;
  serializeJson(doc, output);
  return output;
}
