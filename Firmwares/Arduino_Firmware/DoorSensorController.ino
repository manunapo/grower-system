// Pin analogico de entrada para el sensor de la puerta
//int doorSensorPin = 2;
int cantSensors = 7;
int sensors[7] = {A0,A1,A2,2,3,4,5};
int closedSensors[7];

void setUpDoorSensor(){
  for(int i = 0; i < cantSensors; i++){
    pinMode(sensors[i], INPUT);
  }
  updateSensors();
}

void updateSensors(){
  for(int i = 0; i < cantSensors; i++){
    if(digitalRead(sensors[i]) == LOW){
      closedSensors[i] = LOW;
    }else{
      closedSensors[i] = HIGH;
    }
    Serial.print("Sensor :");Serial.print(sensors[i]);
    if(closedSensors[i] == LOW){
      Serial.println(" - Encendido");
    }else{
      Serial.println(" - Apagado");
    }
  }
}

boolean isAnyDoorOpen(){
 boolean isOpen = false;
 int i = 0;
 while(!isOpen && i < cantSensors){
  if(closedSensors[i] == LOW && digitalRead(sensors[i]) == HIGH){
    isOpen = true;
  }
  i++;
 }
 return isOpen;
}
