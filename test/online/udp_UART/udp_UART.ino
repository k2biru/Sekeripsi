#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UART_BAUD 31250
#define packTimeout 0 // ms (if nothing more on UART, then send packet)
#define bufferSize 8192

const char *ssid = "MIDI";  // Your ROUTER SSID
const char *pw = "MIDIMIDI"; // and WiFi PASSWORD
const int port = 1112;

WiFiUDP udp;
IPAddress remoteIp;

uint8_t buf1[bufferSize];
uint8_t i1=0;

uint8_t buf2[bufferSize];
uint8_t i2=0;

void setup() {
delay(500);
  
  Serial.begin(UART_BAUD);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  Serial.println("Starting UDP Server");
  udp.begin(port); // start UDP server 

}
void loop() {
if(Serial.available()) {

    
    while(1) {
      if(Serial.available()) {
        buf2[i2] = Serial.read(); // read char from UART
        if(i2<bufferSize-1) {
          i2++;
        }
      } else {
        //delayMicroseconds(packTimeoutMicros);
        //Serial.println("dl");
        delay(packTimeout);
        if(!Serial.available()) {
          //Serial.println("bk");
          break;
        }
      }
    }
    // now send to WiFi:  
    udp.beginPacket("192.168.43.255", 1112); // remote IP and port
    udp.write(buf2, i2);
    udp.endPacket();
    i2 = 0;
  }

}
