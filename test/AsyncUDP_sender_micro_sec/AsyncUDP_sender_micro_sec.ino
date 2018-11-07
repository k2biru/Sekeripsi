#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

#define BIT_SEND 9
#define _ARRAY   9

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";


AsyncUDP udp;
unsigned int ns = 750;



byte msg[_ARRAY];


void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  Serial.println("I'M OK");
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  msg[0] = 'A';
#ifdef BIT_MULTY
  for (int x = 1; x < _ARRAY; x++)
  {
    msg[x] = ' ';
  }
#endif

  if (udp.connect(IPAddress(255, 255, 255, 255), 1112)) {
    Serial.println("\n UDP connected : 255, 255,255, 255");
  }
  delay(5000);
}

void loop() {
  // Mynr
  udp.write(msg, BIT_SEND);
  msg[0]++;
  delayMicroseconds(ns);

  if (msg[0] > 'Z') {
    msg[0] = 'A';
  }
}
