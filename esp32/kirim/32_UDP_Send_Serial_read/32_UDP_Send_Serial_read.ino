#include "WiFi.h"
#include <WiFiUdp.h>

#define FLAG_BIT_MIDI B10000000
 
const char *ssid = "MIDI";
const char *password = "MIDIMIDI";

unsigned int UDPPort = 1112;

char packetBuffer[255];

WiFiUDP udp;
unsigned int nbr;
byte MIDIData[3];
byte buff;

void setup() {
  Serial.begin(31250);
  //Serial.begin(115200);
  pinMode(2,OUTPUT);
  WiFi.softAP(ssid, password);
 
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  delay(5000);
  //testNote();
 
}
 
void loop() {
    digitalWrite(2,HIGH);
    udp.beginPacket("192.168.4.255", UDPPort);
    udp.printf("%s", MIDIData);
    udp.endPacket();
    digitalWrite(2,LOW);
    //nbr++;
    delay(1000);
  }

void testNote(){
  MIDIData[0]= B10010000;
  MIDIData[1]= B01000101;
  MIDIData[2]= B01100101;
}
