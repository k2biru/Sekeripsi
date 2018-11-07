void setup() {
  // put your setup code here, to run once:
Serial.begin(31250);
delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
playNote();
delay(1000);
}
void playNote(){
  Serial.write(B10010000);
  Serial.write(B01000101);
  Serial.write(B01100101);
}

