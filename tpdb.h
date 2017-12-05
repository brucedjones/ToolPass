#ifndef TPDB_h
#define TPDB_h

#include "Arduino.h"

#include <SoftwareSerial.h>
#include <EDB.h>
#include <SPI.h>
#include <SD.h>

#define SD_PIN 5  // SD Card CS pin
#define TABLE_SIZE 8192

// Hate the use of globals here, blame the EDB library
// also blame arduino for the use of static...
void writer (unsigned long address, byte data);
byte reader (unsigned long address);
static File tpdbFile;

typedef struct {
    char uid[16];
    char name[16];
    bool authorized;
    unsigned int balance;
} User;

class TPDB{
	public:
		TPDB(char *fname, Print *debugPrinter);
		~TPDB();
		void DeleteUser(User r);
        void ModifyUser(User r);
        void AddUser(User r);
        User GetUser(char *uid);

	private:
		Print *debugPrinter;
        EDB *db;
        
        void printError(EDB_Status err);
};

#endif
