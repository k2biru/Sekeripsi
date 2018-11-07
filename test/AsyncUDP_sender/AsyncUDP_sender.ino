#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;

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
    delay(10);
    byte msg[10];
    //sprintf(msg, "Yoi haha");
    msg[0] ='Y';
    msg[1] ='o';
    msg[2] ='i';
    msg[3] =' ';
    msg[4] ='h';
    msg[5] ='a';
    msg[6] ='h';
    msg[7] ='a';
    
    msg[3]= NULL;
    msg[2] = 32;
    udp.write(msg, 3);
    //char *str = (char*)msg;
    //udp.print((char*)msg);
    //udp.writeTo(msg,2,IPAddress(192,168,43,255),1112);
    //udp.printf(msg,3);
    nbr++;
}
 

