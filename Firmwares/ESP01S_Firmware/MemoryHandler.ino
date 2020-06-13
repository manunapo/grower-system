
#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)

int addrHourWatering = 0;
int addrMinuteWatering = 1;
int addrSecondsWatering = 2;
int addrDayWatering = 3;

int addrHourLighting = 4;
int addrMinuteLighting = 5;
int addrSecondsLighting = 6;
int addrDayLighting = 7;

void setUpMemoryHandler(){
  EEPROM.begin(512);
}

void saveLastWatering(int hour, int minute, int seconds, int day){
  EEPROM.write(addrHourWatering, hour);
  EEPROM.write(addrMinuteWatering, minute);
  EEPROM.write(addrSecondsWatering, seconds);
  EEPROM.write(addrDayWatering, day);
  EEPROM.commit();
}

String getLastWateringTime(){
  String hour =(String) EEPROM.read(addrHourWatering);
  String minute = (String)EEPROM.read(addrMinuteWatering);
  String seconds = (String)EEPROM.read(addrSecondsWatering);
  String day = (String)EEPROM.read(addrDayWatering);
  return hour + ":" + minute + ":" + seconds + " d:" + day; 
}

String getLastLightingTime(){
  String hour = (String) EEPROM.read(addrHourLighting);
  String minute = (String)EEPROM.read(addrMinuteLighting);
  String seconds = (String)EEPROM.read(addrSecondsLighting);
  String day = (String)EEPROM.read(addrDayLighting);
  return hour + ":" + minute + ":" + seconds + " d:" + day; 
}

int getLastDayOfWatering(){
  return EEPROM.read(addrDayWatering);;
}

void saveLastLighting(int hour, int minute, int seconds, int day){
  EEPROM.write(addrHourLighting, hour);
  EEPROM.write(addrMinuteLighting, minute);
  EEPROM.write(addrSecondsLighting, seconds);
  EEPROM.write(addrDayLighting, day);
  EEPROM.commit();
}

int getLastDayOfLighting(){
  return EEPROM.read(addrDayLighting);;
}