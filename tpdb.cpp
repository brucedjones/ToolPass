#include "tpdb.h"

void writer (unsigned long address, byte data) {
    digitalWrite(13, HIGH);
    tpdbFile.seek(address);
    tpdbFile.write(data);
    tpdbFile.flush();
    digitalWrite(13, LOW);
}

byte reader (unsigned long address) {
    digitalWrite(13, HIGH);
    tpdbFile.seek(address);
    byte b = tpdbFile.read();
    digitalWrite(13, LOW);
    return b;
}

TPDB::TPDB(char *fname, Print *debugPrinter){
    this->debugPrinter = debugPrinter;

    db = new EDB(&writer, &reader);

    if (!SD.exists("/db")) {
        debugPrinter->println("Dir for Db files does not exist, creating...");
        SD.mkdir("/db");
    }

    if (SD.exists(fname)) {

        tpdbFile = SD.open(fname, FILE_WRITE);

        // Sometimes it wont open at first attempt, espessialy after cold start
        // Let's try one more time
        if (!tpdbFile) {
            tpdbFile = SD.open(fname, FILE_WRITE);
        }

        if (tpdbFile) {
            debugPrinter->println("Openning current table... ");
            EDB_Status result = db->open(0);
            if (result == EDB_OK) {
                debugPrinter->println("DONE");
            } else {
                debugPrinter->println("ERROR");
                debugPrinter->println("Did not find database in the file " + String(fname));
                debugPrinter->println("Creating new table... ");
                db->create(0, TABLE_SIZE, (unsigned int)sizeof(User));
                debugPrinter->println("DONE");
                return;
            }
        } else {
            debugPrinter->println("Could not open file " + String(fname));
        }
    } else {
        debugPrinter->println("Creating table... ");
        // create table at with starting address 0
        tpdbFile = SD.open(fname, FILE_WRITE);
        db->create(0, TABLE_SIZE, (unsigned int)sizeof(User));
        debugPrinter->println("DONE");
    }
}

TPDB::~TPDB(){
    tpdbFile.close();
    delete db;
}

void TPDB::DeleteUser(User r){
    User res;
    for (int recno = 1; recno <= db->count(); recno++)
    {
        EDB_Status result = db->readRec(recno, EDB_REC res);
        if (result == EDB_OK)
        {
            bool found = true;
            for(int c=0; c<16; c++)
            {
                if(res.uid[c] != r.uid[c]) 
                {
                    found = false;
                    break;
                }
            }
            if(found) db->deleteRec(recno);
        }
        else printError(result);;
    }
}

void TPDB::ModifyUser(User r){
    User res;
    for (int recno = 1; recno <= db->count(); recno++)
    {
        EDB_Status result = db->readRec(recno, EDB_REC res);
        if (result == EDB_OK)
        {
            bool found = true;
            for(int c=0; c<16; c++)
            {
                if(res.uid[c] != r.uid[c]) 
                {
                    found = false;
                    break;
                }
            }
            if(found) {
                db->updateRec(recno, EDB_REC r);
                return;
            }
        }
        else printError(result);
    }

    AddUser(r);
}

void TPDB::AddUser(User r){
    EDB_Status result = db->appendRec(EDB_REC r);
    if (result != EDB_OK) printError(result);
}

User TPDB::GetUser(char *uid){
    User res;
    for (int recno = 1; recno <= db->count(); recno++)
    {
        EDB_Status result = db->readRec(recno, EDB_REC res);
        if (result == EDB_OK)
        {
            bool found = true;
            // debugPrinter->println(res.uid);
            for(int c=0; c<15; c++)
            {
                if(res.uid[c] != uid[c]) 
                {
                    found = false;
                    break;
                }
            }
            if(found) return res;
        }
        else printError(result);
    }

    strcpy(res.uid, "NA             ");
    strcpy(res.name, "NA             ");
    res.authorized = false;
    res.balance = 0;

    debugPrinter->println("User not found:");
    debugPrinter->println(uid);
    return res;
}

void TPDB::printError(EDB_Status err)
{
    debugPrinter->print("ERROR: ");
    switch (err)
    {
        case EDB_OUT_OF_RANGE:
            debugPrinter->println("Recno out of range");
            break;
        case EDB_TABLE_FULL:
            debugPrinter->println("Table full");
            break;
        case EDB_OK:
        default:
            debugPrinter->println("OK");
            break;
    }
}
