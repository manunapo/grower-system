int pinValidTrans = A3;
int pinActivate = 9;
int pinDesactivate = A4;
int pinAlert = 13;
int lastDetected = 0;

void setUpRemoteControl(){
    pinMode(pinValidTrans, INPUT);
    pinMode(pinActivate, INPUT);
    pinMode(pinDesactivate, INPUT);
    pinMode(pinAlert, INPUT);
}

void checkKeys(){
    int toReturn = 0;
    if(digitalRead(pinValidTrans)){  
        if (digitalRead(pinActivate) && lastDetected != 1){
            stateActivate();
            lastDetected = 1;
        }
        if (digitalRead(pinDesactivate) && lastDetected != 2) {
            stateDesactivate();
            lastDetected = 2;            
        }
        if (digitalRead(pinAlert) && lastDetected != 3){
            //toReturn = 3;
            lastDetected = 3;            
        }
     } 
}

void resetKeys(){
  lastDetected = 0;
}

