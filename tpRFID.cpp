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
	rfid = new RFID(SS_PIN, RST_PIN);
	rfid->init();
}

tpRFID::~tpRFID()
{
  SPI.begin(); 
  rfid->init();
}

void tpRFID::CheckCard()
{
	if (rfid->isCard()) {

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

                hold = true;

            }

          }
    } else {
      hold = false;
    }
    
    rfid->halt();
}