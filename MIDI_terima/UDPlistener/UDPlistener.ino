#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define LED_PIN 2

unsigned int UDPPort = 1112;

char packetBuffer[255];

WiFiUDP Udp;

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(31250);
  midiSysReset();
  delay(5000);
  midiSysReset();
  //Serial.begin (115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin("MIDI", "MIDIMIDI");
  //Serial.println();
  //Serial.print("Wait for WiFi");
  noteOn(0x90,0x1E,0x45);
  delay(200);
  noteOn(0x90,0x3D,0x45);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    //Serial.print(".");
    noteOn(0x90,0x54,0x45);
  }
  //Serial.println();
  //Serial.println("WiFi connected");
  //Serial.println("IP : " + WiFi.localIP().toString());
  Udp.begin(UDPPort);



}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    digitalWrite(LED_PIN, HIGH);
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.print(packetBuffer);
    digitalWrite(LED_PIN, LOW);
  }
}
void noteOn(int cmd, int pitch, int velocity){
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void midiSysReset(){
  Serial.write(0xFF);
}

