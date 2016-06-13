#include <Arduino.h>
#include <SoftwareSerial.h>
#include "toolpassServer.h"

// These are the SSID and PASSWORD to connect to your Wifi Network
//  put details appropriate for your network between the quote marks,
//  eg  #define ESP8266_SSID "YOUR_SSID"
#define wifiSSID  "RQ3R7"
#define wifiPASS  "M3VTVM7XDT388ZR4"

ToolpassServer *toolpass;



void setup()
{
  Serial.begin(115200); // Used for debug output

  toolpass = new ToolpassServer(8,7,wifiSSID, wifiPASS, &Serial);
  
  // A blank line just for debug formatting 
  Serial.println();
}

void loop()
{
  //toolpass->Test();
  bool auth = toolpass->ToolOn("bbb",2);
  Serial.println(auth);
  //toolpass->ToolOff("bbb",1);
  //toolpass->Log("bbb",1,59.01,22.43);
  delay(5000); 
}
