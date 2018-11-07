#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;

#define MAX_BUFF_SIZE     280
byte buff[MAX_BUFF_SIZE];
unsigned int i, j;

void setup() {
  //Serial.begin (115200);
  Serial.begin (31250);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  if (udp.connect(IPAddress(192, 168, 43, 255), 1112)) {
    Serial.println("\n UDP connected : 192,168,43,255");
  }
  delay(1000);
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
  //for (int h = 0; h < i; h++) {
  //  Serial.write(buff[h]);
  //}
  udp.write(buff, i+1);
  yield();
  //Serial.println();
  i = 0;
}

