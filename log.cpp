#include "log.h"


Log::Log(char *fname,  Print *debugPrinter){
    this->debugPrinter = debugPrinter;

    if (!SD.exists("/log")) {
        debugPrinter->println("Dir for log files does not exist, creating...");
        SD.mkdir("/log");
    }

    logFile = SD.open(fname, FILE_WRITE);

    if (!logFile) {
        logFile = SD.open(fname, FILE_WRITE);
    }

    if(!logFile) debugPrinter->println("Could not open file " + String(fname));
}

Log::~Log(){
    logFile.close();
}

void Log::Write(char *msg){
    bool h12, PM, Century;
    logFile.print("[");
	logFile.print(clock.getYear(), DEC);
	logFile.print('-');
    logFile.print(clock.getMonth(Century), DEC);
    logFile.print('-');
    logFile.print(clock.getDate(), DEC);
    logFile.print(' ');
	// then the date
	// Finally the hour, minute, and second
	logFile.print(clock.getHour(h12, PM), DEC);
	logFile.print(':');
	logFile.print(clock.getMinute(), DEC);
	logFile.print(':');
	logFile.print(clock.getSecond(), DEC);
    logFile.print("]");
    logFile.print(' ');
    logFile.println(msg);
    logFile.print('\n');
    logFile.flush();
}