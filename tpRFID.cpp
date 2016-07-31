#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#include "tpRFID.h"
#include "RFID.h"

#define SS_PIN 10
#define RST_PIN 9

tpRFID::tpRFID(HardwareSerial *debugPrinter)
{
	this->debugPrinter = debugPrinter;
	hold = false;
  	SPI.begin(); 
	rfid = new RFID(SS_PIN, RST_PIN);
	rfid->init();
  cardID = new char[10];
}

tpRFID::~tpRFID()
{
}

bool tpRFID::CheckCard()
{
  //debugPrinter->println("Tock");
	if (rfid->isCard()) {
		debugPrinter->println("Card Present");
        if (!hold){
          if (rfid->readCardSerial()) {           
		            debugPrinter->print(rfid->serNum[0],HEX);
                debugPrinter->print("-");
		            debugPrinter->print(rfid->serNum[1],HEX);
                debugPrinter->print("-");
		            debugPrinter->print(rfid->serNum[2],HEX);
                debugPrinter->print("-");
		            debugPrinter->print(rfid->serNum[3],HEX);
                debugPrinter->print("-");
		            debugPrinter->print(rfid->serNum[4],HEX);
                debugPrinter->println(" ");

                utoa(rfid->serNum[0], &cardID[0], HEX);
                utoa(rfid->serNum[1], &cardID[2], HEX);
                utoa(rfid->serNum[2], &cardID[4], HEX);
                utoa(rfid->serNum[3], &cardID[6], HEX);
                utoa(rfid->serNum[4], &cardID[8], HEX);

                /*String code = String(rfid->serNum[0],HEX);
                code = String(code + String(rfid->serNum[1],HEX));
                code = String(code + String(rfid->serNum[2],HEX));
                code = String(code + String(rfid->serNum[3],HEX));
                code = String(code + String(rfid->serNum[4],HEX));

                cardID = new String(code);*/

                hold = true;

                return true;
            }

          }
    } else {
      hold = false;
    }
    
    rfid->halt();
    return false;
}