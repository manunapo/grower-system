#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial ESP01S(7, 8); // TX, RX

void setup()

{
  //Revisar https://github.com/luisllamasbinaburo/Arduino-AsyncSerial
  Serial.begin(115200);

  ESP01S.begin(115200);

}

String content = "";
char character;

void loop()

{

  sendJSONtoESP();
  Serial.println("DATA sent, waiting response: ");
  delay(2000);
  if (ESP01S.available()){

    while (ESP01S.available()) {
      character = ESP01S.read();
      content.concat(character);
    }
    Serial.println("--ESP answered: ");
    Serial.println(content);
    Serial.println("-----------------");
  }
  
  delay(10000);
}

void sendJSONtoESP(){
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
    Serial.println("to send: ");
    Serial.println(output);
    ESP01S.print(output);
}
