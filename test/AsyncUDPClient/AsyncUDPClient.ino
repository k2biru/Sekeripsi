#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;
unsigned int nbr = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  } if (udp.listen(1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.write(packet.data(), packet.length());
    });
  }
}

void loop()
{
  //Orangopo-ngopo
}
