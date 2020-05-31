#include <SoftwareSerial.h>
//TX,RX
SoftwareSerial SIM900(7, 8);
int pinLedSignal_1 = 11;
int pinLedSignal_2 = 12;

String outMessage = "Se activo la alarma";
String destinationNumber = "298315550485";

void setUpGprs(){
  SIM900.begin(19200);
  pinMode(pinLedSignal_1, OUTPUT);
  pinMode(pinLedSignal_2, OUTPUT);
  SIM900.print("AT+CMGF=1\r");  
  delay(1000);
  SIM900.print("AT+CNMI=1,2,0,0,0\r"); 
  delay(1000);
}

void sendMessage(){
  SIM900.print("AT+CMGF=1\r");
  delay(1000);
  SIM900.println("AT + CMGS = \"" + destinationNumber +"\"");
  delay(1000);
  SIM900.print(outMessage);
  delay(1000);
  SIM900.write((char)26); //ctrl+z
  delay(1000);
}

void makeCall(){
  SIM900.println("ATD" + destinationNumber + ";");
  delay(1000);
}

void hangUp(){
   SIM900.println("ATH");
   delay(1000);
}

void getCLTS(){
  SIM900.print("AT+CLTS?\r");
  delay(100);
  Serial.print("CLTS: " + SIM900.read());
}

void getTime(){
  SIM900.print("AT+CCLK?\r");
  delay(100);
  
  Serial.print("Time: ");
  while(SIM900.available() > 0){
      Serial.print( SIM900.read());
  }
   Serial.println();
}

void checkSMS(){
  char incomingChars;
  int counter = 0;
  String msgBody = "";
  if(SIM900.available() >0){
    while(SIM900.available() >0){
      incomingChars = SIM900.read(); //Get the character from the cellular serial port.
      if(counter == 6){
        if(incomingChars != '\n' && incomingChars != '\r'){
          msgBody += incomingChars;
        }      
      }
      if( incomingChars == '"'){
       counter++; 
      }
      Serial.print(incomingChars); //Print the incoming character to the terminal.
    } 
    msgBody.toLowerCase();
    boolean activate = msgBody.equals("activar");
    boolean desactivate = msgBody.equals("desactivar");
    if(activate){
      stateActivate();
      Serial.print("Llego activacion: ");Serial.println(msgBody);
    }
    if(desactivate){
      stateDesactivate();   
      Serial.print("Llego desactivacion: ");Serial.println(msgBody);
    }
  }
}

void checkSignal(){
  SIM900.print("AT+CSQ\r");
  delay(100);
  char incomingChars;
  boolean found = false;
  int signalInt = 0;
  while(SIM900.available() > 0){
      incomingChars = SIM900.read();
      //Serial.print(incomingChars);
      if(incomingChars == ','){
        found = false;
      }
      if(found){
        if(incomingChars == '0' ||
        incomingChars == '1' ||
        incomingChars == '2' ||
        incomingChars == '3' ||
        incomingChars == '4' ||
        incomingChars == '5' ||
        incomingChars == '6' ||
        incomingChars == '7' ||
        incomingChars == '8' ||
        incomingChars == '9')
        signalInt = signalInt * 10 + (incomingChars - '0');
      }
      if(incomingChars == ':'){
        found = true;
      }
  }
  Serial.print("Signal: ");Serial.println(signalInt);
  if( signalInt > 2 && signalInt < 10){
    digitalWrite(pinLedSignal_1, HIGH);
    digitalWrite(pinLedSignal_2, LOW);
  }else if(signalInt >= 10){
    digitalWrite(pinLedSignal_1, HIGH);
    digitalWrite(pinLedSignal_2, HIGH);
  }else{
    digitalWrite(pinLedSignal_1, LOW);
    digitalWrite(pinLedSignal_2, LOW);
  }
}
