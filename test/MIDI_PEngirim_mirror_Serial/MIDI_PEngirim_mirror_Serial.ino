byte buff;
void setup() {

Serial.begin(31250);
pinMode(2, OUTPUT);

}

void loop() {
  while (Serial.available() ) {
    digitalWrite(2, HIGH);
    buff = Serial.read();
    Serial.write (buff);
    digitalWrite(2, LOW);
  }

}
