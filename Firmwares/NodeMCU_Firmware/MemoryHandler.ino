#include <EEPROM.h>

int addrHourWatering = 0;
int addrMinuteWatering = 1;
int addrSecondsWatering = 2;
int addrDayWatering = 3;

int addrHourLighting = 4;
int addrMinuteLighting = 5;
int addrSecondsLighting = 6;
int addrDayLighting = 7;

// Configuration Parameters
int addrHourToWatering = 8;
int addrHourToLighting = 9;

int addrWateringTime = 10;
int addrLightingTime = 11;

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
  int hour = EEPROM.read(addrHourWatering);
  int minute = EEPROM.read(addrMinuteWatering);
  int second = EEPROM.read(addrSecondsWatering);
  String hours = "" + (String)hour;
  String minutes = "" + (String)minute;
  String seconds = "" + (String)second ;
  if( hour < 10){
    hours = "0" + hours;
  }
  if( minute < 10){
    minutes = "0" + minutes;
  }
  if( second < 10){
    seconds = "0" + seconds;
  }
  
  String day = (String)EEPROM.read(addrDayWatering);
  return hours + ":" + minutes + ":" + seconds + " d:" + day; 
}

String getLastLightingTime(){
  int hour = EEPROM.read(addrHourLighting);
  int minute = EEPROM.read(addrMinuteLighting);
  int second = EEPROM.read(addrSecondsLighting);
  String hours = "" + (String)hour;
  String minutes = "" +(String) minute;
  String seconds = "" + (String)second ;
  if( hour < 10){
    hours = "0" + hours;
  }
  if( minute < 10){
    minutes = "0" + minutes;
  }
  if( second < 10){
    seconds = "0" + seconds;
  }
  
  String day = (String)EEPROM.read(addrDayLighting);
  return hours + ":" + minutes + ":" + seconds + " d:" + day; 
}

int getLastDayOfWatering(){
  return EEPROM.read(addrDayWatering);
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

void saveConfigurationParameters(int hourToW, int hourToL, int waterTime, int lighTime){
  EEPROM.write(addrHourToWatering, hourToW);
  EEPROM.write(addrHourToLighting, hourToL);
  EEPROM.write(addrWateringTime, waterTime);
  EEPROM.write(addrLightingTime, lighTime);
  EEPROM.commit();
}

int getHourToWatering(){
  return EEPROM.read(addrHourToWatering);
}

int getHourToLighting(){
  return EEPROM.read(addrHourToLighting);
}

int getWateringTime(){
  return EEPROM.read(addrWateringTime);
}

int getLightingTime(){
  return EEPROM.read(addrLightingTime);
}
