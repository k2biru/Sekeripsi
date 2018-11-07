#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;
unsigned int nbr=0;

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.connect(IPAddress(192,168,43,255), 1112)) {
        Serial.println("\n UDP connected : 192,168,43,255");
       }
        //Send unicast
        udp.print("Aku Siap /n");

}

void loop()
{
    delay(1000);
    char msg[10];
    sprintf(msg, "Yoi haha");
    msg[3]= NULL;
    udp.print(msg);
    //udp.writeTo(msg,2,IPAddress(192,168,43,255),1112);
    //udp.printf(msg,3);
    nbr++;
}
