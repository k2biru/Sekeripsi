#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

unsigned int UDPPort = 1112;

char packetBuffer[255];

WiFiUDP Udp;

int nbr;

void setup() {
  Serial.begin (115200);

  WiFi.softAP("MIDI","MIDIMIDI");
  Udp.begin(UDPPort);
  Serial.println ();
  Serial.println ("Buat AP" );
}

void loop() {
  char buff[64];
  // put your main code here, to run repeatedly:
Udp.beginPacket("192.168.4.255", UDPPort);
//Serial.print ("Send data" );
//Serial.println (nbr);
Udp.write(nbr);
Udp.endPacket();
nbr++;
yield;
delay (100); 
}
