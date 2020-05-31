#include <EEPROM.h>

//Memory will be stored in the following way:
//Staring from STORAGE_START 
//d,d,m,m,y,y,d,d,m,m,y,y,f
//f char will be used to search the last date.
//f will be moved to las position every new write

void saveDate( int day, int month, int year){
  int startAddress = foundStartAddress();
  EEPROM.put( startAddress, day);
  EEPROM.put( startAddress + sizeof(int), month);
  EEPROM.put( startAddress + sizeof(int) + sizeof(int), year);
  EEPROM.put( startAddress + sizeof(int) + sizeof(int) + sizeof(int), 'f');
}

String getLastDate(){
  int startAddress = foundStartAddress();
  int day;
  int month;
  int year;
  EEPROM.get( startAddress, day);
  EEPROM.get( startAddress + sizeof(int), month);
  EEPROM.get( startAddress + sizeof(int) + sizeof(int), year);
  return day + "" + month + year;
}



int foundStartAddress(){
  int startAddress = 0;
  boolean foundF = false;
  while( startAddress < EEPROM.length() && !foundF){
    if(  EEPROM.read(startAddress) == 'f'){
      foundF = true;
    }
    startAddress++;
  }
  if( !foundF){
    startAddress = 0;
  }
  return startAddress;
}
