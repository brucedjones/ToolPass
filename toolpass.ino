#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h> 
#include "toolpassServer.h"
#include "tpRFID.h"
#include "ssid.h"
#include "inPin.h"

ToolpassServer *toolpass;
tpRFID *rfid;
InPin *laserPin;

bool machineOn = false;
int thisTool = 1;

void setup()
{
  Serial.begin(115200); // Used for debug output

  toolpass = new ToolpassServer(8,7,wifiSSID, wifiPASS, &Serial, thisTool);
  rfid = new tpRFID(&Serial);
  laserPin = new InPin(A0, 3.3, 1000000.0, 100000.0, 10, &Serial);
  
  // A blank line just for debug formatting 
  Serial.println();

  // Relay
  //pinMode(2, OUTPUT);           // set pin to input
}


void loop()
{
  // Relay
  /*if(flip)
  {
    digitalWrite(2, HIGH);       // turn on pullup resistors
  } else
  {
    digitalWrite(2, LOW);       // turn on pullup resistors
  }
  flip =! flip;*/
  //toolpass->Test();
  //bool auth = toolpass->ToolOn("bbb",1);
  //Serial.println(auth);
  //toolpass->ToolOff("bbb",1);
  //toolpass->Log("bbb",1,59.01,22.43);

  // Actual toolpass code below  
  bool cardScanned = rfid->CheckCard();
  if(cardScanned) {

    char *cardID = rfid->cardID;

    if(!machineOn){    
      //delay(5000);
      bool auth = toolpass->ToolOn(cardID);

      if(auth) {
        Serial.println("Switching machine on");
        machineOn = true;
      } else {
        Serial.println("Not authorized");
      }

    } else {
      Serial.println("Switching machine off");
      
      float usage = laserPin->GetUsage();
      if(usage>0.0) toolpass->Log(usage,21.0);
      
      toolpass->ToolOff();
      machineOn = false;
    }
 
  }

  if(machineOn) {
    laserPin->Poll();
  }

}
