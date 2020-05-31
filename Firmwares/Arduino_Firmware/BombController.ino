int pinLedBomb = 10;
boolean bombing = false;

void setUpBomb(){
   pinMode(pinLedBomb, OUTPUT);
   bombing = false;
}


void bombOn(){
   bombing = true;
   digitalWrite(pinLedBomb, HIGH); 
}

void bombOff(){
   bombing = false;
   digitalWrite(pinLedBomb, LOW); 
}

void ledOn(){
  digitalWrite(pinLedBomb, HIGH);
}

void ledOff(){
  digitalWrite(pinLedBomb, LOW); 
}

boolean mayStartWatering(){
  
}

void startWatering(){
  
}
 
boolean mayStopWatering(){

}

void stopWatering(){
  
}
