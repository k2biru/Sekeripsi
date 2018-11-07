#define MAX_BUFF_SIZE     280
byte buff[MAX_BUFF_SIZE];
unsigned int i, j;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin (115200);
  Serial.begin (31250);
  //Serial.println("I'm Ready");
}

void loop() {
  if (Serial.available()) {
    while (1) {
      if (Serial.available()) {
        buff[i] = Serial.read();
        if (i < MAX_BUFF_SIZE - 1) {
          i++;
        } else {
          break;
        }
      } else {
        //wait time
        break;
      }
    }
    kirim();
  }
}

void kirim() {
  //Serial.print("i = "); Serial.println(i);
  //Serial.print("Data = ");
  for (int h = 0; h < i; h++) {
    Serial.write(buff[h]);
  }
  //Serial.println();
  i = 0;
}

