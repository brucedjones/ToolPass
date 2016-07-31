#include <Arduino.h>
#include <SoftwareSerial.h>
#include "simpleJSON.h"

#include "toolpassServer.h"

#include "ESP8266_Serial.h"
#include "ESP8266_Simple.h"

ToolpassServer::ToolpassServer(short rxPin, short txPin, const char *SSID, const char *Password, Print *debugPrinter, int tool_id){
	wifi = new ESP8266_Simple(rxPin,txPin);
	wifi->begin(9600);
	wifi->setupAsWifiStation(SSID, Password, debugPrinter);
	this->debugPrinter = debugPrinter;
	this->tool_id = tool_id;
}

ToolpassServer::~ToolpassServer()
{
	delete wifi;
}

void ToolpassServer::Test()
{
  
  char buffer[500]; // Remember, SRAM is very limited, think carefully about
                    // how big a buffer you really need!

  

  memcpy(buffer, 0, sizeof(buffer));     // Ensure the buffer is empty first!
  strncpy_P(buffer, PSTR("/api/v1"), sizeof(buffer)-1);
  

  debugPrinter->print("Requesting ");
  debugPrinter->print(buffer);
  debugPrinter->print(": ");
  
  unsigned int httpResponseCode = 
    wifi->GET
    (
      80,                     // The Port to Connect to (80 is the usual "http" port)
      buffer,                 // Your buffer which currently contains the path to request
      sizeof(buffer),         // The size of the buffer
      F("www.toolpass.io"), // Optional hostname you are connecting to(* see below)
      1                       // Get from line 2 of the body, no headers (use 0 to get headers)
                              // responses often have a leading newline, hence starting 
                              // from line 2 here, adjust as necessary
    );
  
  // Now let's interpret the response code
  if(httpResponseCode == 200 || httpResponseCode == ESP8266_OK)
  {
    // Our request was successfull and the response can be found in the buffer
    debugPrinter->println("OK");
    debugPrinter->println(buffer);
    /*StaticJsonBuffer<500> jsonBuffer;
    JsonObject& res = jsonBuffer.parseObject(buffer);
    float version = res["api_version"];
    const char *status = res["status"];
    debugPrinter->println("API Version");
    debugPrinter->println(version);
    debugPrinter->println("Status");
    debugPrinter->println(status);*/
  }
  else
  {
    // Oops, something went wrong.
    if(httpResponseCode < 100)
    {
      // And it's on our end, but what was it?  Well we can find out easily.      
      wifi->debugPrintError((byte)httpResponseCode, debugPrinter);
    }
    else
    {
      // It's probably a server problem
      debugPrinter->print("HTTP Status ");
      debugPrinter->println(httpResponseCode);
    }
  }
}

bool ToolpassServer::ToolOn(char *user_id)
{
  this->user_id = user_id;
  bool authorized = false;
  char buffer[500]; // Remember, SRAM is very limited, think carefully about
                    // how big a buffer you really need!

  memcpy(buffer, 0, sizeof(buffer));     // Ensure the buffer is empty first!
  strncpy_P(buffer, PSTR("/api/v1/tool-on"), sizeof(buffer)-1);
  
// For fun, let's add a variable parameter to our fixed request path, first the
  // parameter name itself, which is fixed we will store in FLASH, and then
  // and then our value, in this case the result of millis which we insert
  // into the string with ltoa();


  strncpy_P(buffer+strlen(buffer), PSTR("?card="), sizeof(buffer)-strlen(buffer)-1);
  strncpy(buffer+strlen(buffer),user_id,64-strlen(buffer));

  strncpy_P(buffer+strlen(buffer), PSTR("&tool_id="), sizeof(buffer)-strlen(buffer)-1);
  ltoa(tool_id, buffer+strlen(buffer), 10); // Note "10" is Base10, not a length
  //dtostrf(val, 4, 6, buff); //4 is mininum width, 6 is precision

  debugPrinter->print("Requesting ");
  debugPrinter->print(buffer);
  debugPrinter->print(": ");
  
  short numTries = 0;
  bool success = false;

  while(!success && numTries<5)
  {

	  unsigned int httpResponseCode = 
	    wifi->GET
	    (
	      80,                     // The Port to Connect to (80 is the usual "http" port)
	      buffer,                 // Your buffer which currently contains the path to request
	      sizeof(buffer),         // The size of the buffer
	      F("www.toolpass.io"), // Optional hostname you are connecting to(* see below)
	      1                       // Get from line 2 of the body, no headers (use 0 to get headers)
	                              // responses often have a leading newline, hence starting 
	                              // from line 2 here, adjust as necessary
	    );
	  
	  // Now let's interpret the response code
	  if(httpResponseCode == 200 || httpResponseCode == ESP8266_OK)
	  {
	    // Our request was successfull and the response can be found in the buffer
	    debugPrinter->println("OK");
	    debugPrinter->println(buffer);
	    char * tool_on = SimpleJSON::GetStrKey("status",6,buffer,200);
	    if(tool_on[0] == 'o' && tool_on[1] == 'k') authorized = true;
	    debugPrinter->println("Tool on:");
	    debugPrinter->println(tool_on);
	    success = true;
	  }
	  else
	  {
	    // Oops, something went wrong.
	    if(httpResponseCode < 100)
	    {
	      // And it's on our end, but what was it?  Well we can find out easily.      
	      wifi->debugPrintError((byte)httpResponseCode, debugPrinter);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	    else
	    {
	      // It's probably a server problem
	      debugPrinter->print("HTTP Status ");
	      debugPrinter->println(httpResponseCode);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	  }
  }

  return authorized;
}

void ToolpassServer::ToolOff()
{
	char buffer[500]; // Remember, SRAM is very limited, think carefully about
                    // how big a buffer you really need!

  memcpy(buffer, 0, sizeof(buffer));     // Ensure the buffer is empty first!
  strncpy_P(buffer, PSTR("/api/v1/tool-off"), sizeof(buffer)-1);

  strncpy_P(buffer+strlen(buffer), PSTR("?card="), sizeof(buffer)-strlen(buffer)-1);
  strncpy(buffer+strlen(buffer),user_id,64-strlen(buffer));
  
  strncpy_P(buffer+strlen(buffer), PSTR("&tool_id="), sizeof(buffer)-strlen(buffer)-1);
  ltoa(tool_id, buffer+strlen(buffer), 10); // Note "10" is Base10, not a length

  debugPrinter->print("Requesting ");
  debugPrinter->print(buffer);
  debugPrinter->print(": ");
  
  short numTries = 0;
  bool success = false;

  while(!success && numTries<5)
  {

	  unsigned int httpResponseCode = 
	    wifi->GET
	    (
	      80,                     // The Port to Connect to (80 is the usual "http" port)
	      buffer,                 // Your buffer which currently contains the path to request
	      sizeof(buffer),         // The size of the buffer
	      F("www.toolpass.io"), // Optional hostname you are connecting to(* see below)
	      1                       // Get from line 2 of the body, no headers (use 0 to get headers)
	                              // responses often have a leading newline, hence starting 
	                              // from line 2 here, adjust as necessary
	    );
	  
	  // Now let's interpret the response code
	  if(httpResponseCode == 200 || httpResponseCode == ESP8266_OK)
	  {
	    // Our request was successfull and the response can be found in the buffer
	    debugPrinter->println("OK");
	    debugPrinter->println(buffer);
	    /*StaticJsonBuffer<500> jsonBuffer;
	    JsonObject& res = jsonBuffer.parseObject(buffer);
	    const char *tool_off = res["tool_off"];
	    debugPrinter->println("Tool off:");
	    debugPrinter->println(tool_off);*/
	    success = true;
	  }
	  else
	  {
	    // Oops, something went wrong.
	    if(httpResponseCode < 100)
	    {
	      // And it's on our end, but what was it?  Well we can find out easily.      
	      wifi->debugPrintError((byte)httpResponseCode, debugPrinter);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	    else
	    {
	      // It's probably a server problem
	      debugPrinter->print("HTTP Status ");
	      debugPrinter->println(httpResponseCode);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	  }
  }
}

void ToolpassServer::Log(float seconds, float temperature)
{
	char buffer[500]; // Remember, SRAM is very limited, think carefully about
                    // how big a buffer you really need!

  

  memcpy(buffer, 0, sizeof(buffer));     // Ensure the buffer is empty first!
  strncpy_P(buffer, PSTR("/api/v1/tool-log"), sizeof(buffer)-1);
  
  strncpy_P(buffer+strlen(buffer), PSTR("?card="), sizeof(buffer)-strlen(buffer)-1);
  strncpy(buffer+strlen(buffer),user_id,64-strlen(buffer));

  strncpy_P(buffer+strlen(buffer), PSTR("&tool_id="), sizeof(buffer)-strlen(buffer)-1);
  ltoa(tool_id, buffer+strlen(buffer), 10); // Note "10" is Base10, not a length

  strncpy_P(buffer+strlen(buffer), PSTR("&seconds="), sizeof(buffer)-strlen(buffer)-1);
  dtostrf(seconds, 5, 2, buffer+strlen(buffer)); //4 is mininum width, 6 is precision

  strncpy_P(buffer+strlen(buffer), PSTR("&temperature="), sizeof(buffer)-strlen(buffer)-1);
  dtostrf(temperature, 5, 2, buffer+strlen(buffer)); //4 is mininum width, 6 is precision

  debugPrinter->print("Requesting ");
  debugPrinter->print(buffer);
  debugPrinter->print(": ");

  short numTries = 0;
  bool success = false;

  while(!success && numTries<5)
  {
  
	  unsigned int httpResponseCode = 
	    wifi->GET
	    (
	      80,                     // The Port to Connect to (80 is the usual "http" port)
	      buffer,                 // Your buffer which currently contains the path to request
	      sizeof(buffer),         // The size of the buffer
	      F("www.toolpass.io"), // Optional hostname you are connecting to(* see below)
	      1                       // Get from line 2 of the body, no headers (use 0 to get headers)
	                              // responses often have a leading newline, hence starting 
	                              // from line 2 here, adjust as necessary
	    );
	  
	  // Now let's interpret the response code
	  if(httpResponseCode == 200 || httpResponseCode == ESP8266_OK)
	  {
	    // Our request was successfull and the response can be found in the buffer
	    debugPrinter->println("OK");
	    debugPrinter->println(buffer);
	    /*StaticJsonBuffer<500> jsonBuffer;
	    JsonObject& res = jsonBuffer.parseObject(buffer);
	    const char *status = res["status"];
	    debugPrinter->println("Logging:");
	    debugPrinter->println(status);*/
	  }
	  else
	  {
	    // Oops, something went wrong.
	    if(httpResponseCode < 100)
	    {
	      // And it's on our end, but what was it?  Well we can find out easily.      
	      wifi->debugPrintError((byte)httpResponseCode, debugPrinter);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	    else
	    {
	      // It's probably a server problem
	      debugPrinter->print("HTTP Status ");
	      debugPrinter->println(httpResponseCode);
	      debugPrinter->println("Retrying");
	      numTries++;
	      delay(5000);
	    }
	  }
  }
}

