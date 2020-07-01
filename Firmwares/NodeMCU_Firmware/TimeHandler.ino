#include <WiFiUdp.h>
#include <NTPClient.h>
WiFiUDP ntpUDP;
const char* ntpServerName = "time.google.com";

NTPClient timeClient(ntpUDP, ntpServerName, -10800, 60000);

// -1 = do not do the action. Default values.
// Definitive values will be loaded from memory
int hourToWatering = 9;
int hourToLighting = 9;

void setUpTimeHandler(){
  timeClient.begin();
  hourToWatering = getHourToWatering();
  hourToLighting = getHourToLighting();
}

void updateTime(){
  timeClient.update();
}

boolean mayStartWatering(){
  boolean toR = false; 
  if( (hourToWatering != -1) && !todayHasWatered()){
    if( timeClient.getHours() == hourToWatering){
      toR = true;
      saveLastWatering( timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), timeClient.getDay());
    }
  }
  return toR;
}

boolean todayHasWatered(){
  boolean toR = false;
  if( getLastDayOfWatering() ==  timeClient.getDay()){  //0 is Sunday
    toR = true;
  }
  return toR;
}

boolean mayStartLighting(){
  boolean toR = false;
  if( (hourToLighting != -1) && !todayHasLighted()){
    if( timeClient.getHours() == hourToLighting){
      toR = true;
      saveLastLighting( timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), timeClient.getDay());
    }
  }
  return toR;
}

boolean todayHasLighted(){
  boolean toR = false;
  if( getLastDayOfLighting() ==  timeClient.getDay()){  //0 is Sunday
    toR = true;
  }
  return toR;
}

String getTime(){
  return timeClient.getFormattedTime();
}

void saveTime(){
  saveLastWatering( timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), timeClient.getDay());
  saveLastLighting( timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), timeClient.getDay());
}
