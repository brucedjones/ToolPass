#ifndef LOG_h
#define LOG_h

#include "Arduino.h"

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>

#define SD_PIN 5  // SD Card CS pin
static File logFile;

class Log {
	public:
		Log (char *fname, Print *debugPrinter);
		~Log ();
		void Write(char *msg);

	private:
		Print *debugPrinter;
        File logFile;
		
		DS3231 clock;
};

#endif
