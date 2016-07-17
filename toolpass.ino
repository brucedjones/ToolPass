#include <Arduino.h>
#include <SoftwareSerial.h>
#include "toolpassServer.h"
#include "tpRFID.h"
#include "ssid.h"

ToolpassServer *toolpass;
tpRFID *rfid;


void setup()
{
  Serial.begin(115200); // Used for debug output

  toolpass = new ToolpassServer(8,7,wifiSSID, wifiPASS, &Serial);
  rfid = new tpRFID(&Serial);
  
  // A blank line just for debug formatting 
  Serial.println();
}

void loop()
{
  //toolpass->Test();
  //bool auth = toolpass->ToolOn("bbb",1);
  //Serial.println(auth);
  //toolpass->ToolOff("bbb",1);
  //toolpass->Log("bbb",1,59.01,22.43);
  rfid->CheckCard();
}
