/*
*/
#define FLAG_BIT_MIDI B10000000


byte byteSerial[3];
byte buff;
byte i=0;
void setup() {
  //Serial.begin(31250);
  Serial.begin(115200);
}

void loop() {
while (Serial.available() ) {
  buff = Serial.read();
  if (buff&FLAG_BIT_MIDI){
    //Serial.print("Y");
    kirim();
    kosong();
     byteSerial[0]= buff;
  }else{
    //Serial.print("C");
    i++;
    byteSerial[i] = buff;
  }
}
}

void serialEvent() {

}



void kirim(){
  Serial.print("Kr-> ");
  Serial.print(byteSerial[0]);
  Serial.print(byteSerial[1]);
  Serial.println(byteSerial[2]);
}
void kosong(){
  i=0;
  byteSerial [0]= NULL;
  byteSerial [1]= NULL;
  byteSerial [2]= NULL;
}


