int pinBomb = 6;
int pinLedBomb = 10;

void setUpBomb() {
  pinMode(pinBomb, OUTPUT);
  pinMode(pinLedBomb, OUTPUT);
}

void startBombing() {
  digitalWrite(pinBomb, HIGH);
  digitalWrite(pinLedBomb, HIGH);
  Serial.println("START BOMB");
}

void stopBombing() {
  digitalWrite(pinBomb, LOW);
  digitalWrite(pinLedBomb, LOW);
  Serial.println("STOP BOMB");
}
