int pinBomb = 6;
int pinLedBomb = 10;

void setUpBomb() {
  digitalWrite(pinBomb, HIGH);
  digitalWrite(pinLedBomb, HIGH);
  pinMode(pinBomb, OUTPUT);
  pinMode(pinLedBomb, OUTPUT);
}


void startBombing() {
  digitalWrite(pinBomb, LOW);
  digitalWrite(pinLedBomb, LOW);
  Serial.println("START BOMB");
}

void stopBombing() {
  digitalWrite(pinBomb, HIGH);
  digitalWrite(pinLedBomb, HIGH);
  Serial.println("STOP BOMB");
}
