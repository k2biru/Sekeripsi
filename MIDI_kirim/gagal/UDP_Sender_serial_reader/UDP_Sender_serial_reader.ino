#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "midi_type.h"


#define LED_INDIKATOR 2

unsigned int UDPPort = 1112;

WiFiUDP Udp;

unsigned int nbr;

void setup() {
  Serial.begin(31250);
  //Serial.begin (115200);
  pinMode(LED_INDIKATOR, OUTPUT);

  //WiFi.mode(WIFI_AP);
  //WiFi.softAP("MIDI","MIDIMIDI");

  WiFi.mode(WIFI_STA);
  WiFi.begin("MIDI", "MIDIMIDI");
  Serial.println ("Connect to MIDI" );
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP : " + WiFi.localIP().toString());

  Udp.begin(UDPPort);
}

void loop() {
  //kirim();
  //delay(10);
  while (Serial.available() ) {
    readSerial_(Serial.read());
  }
}
void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);
  Serial.print("Kr-> ");
  Udp.beginPacket("192.168.43.255", UDPPort);
  //Serial.print ("Send data" );
  //Udp.printf("%d", nbr);
  Udp.write(_midiMessage,3);
  //Udp.printf("%d", _midiMessage);
  Udp.endPacket();
  Serial.println (nbr);
  //nbr++;
  yield;
  digitalWrite(LED_INDIKATOR, LOW);
}
void kosongkan() {
  for (unsigned int i = 0; i < (SYS_EX_MAX_SIZE + 1 + 3); i++) {
    _midiMessage[i] = NULL;
  }
}

