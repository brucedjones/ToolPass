#ifndef TP_RFID_h
#define TP_RFID_h

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "RFID.h"

class tpRFID {
	public:
		tpRFID(HardwareSerial *debugPrinter);
		~tpRFID();

		void CheckCard();
	private:
		bool hold;
		RFID *rfid;
		Print *debugPrinter;
};

#endif