#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "toolpassServer.h"

#include "ESP8266_Serial.h"
#include "ESP8266_Simple.h"

ToolpassServer::ToolpassServer(short rxPin, short txPin, const char *SSID, const char *Password, Print *debugPrinter){
	wifi = new ESP8266_Simple(rxPin,txPin);
	wifi->begin(9600);
	wifi->setupAsWifiStation(SSID, Password, debugPrinter);
	this->debugPrinter = debugPrinter;
}

ToolpassServer::~ToolpassServer()
{
	delete wifi;
}

void ToolpassServer::Test(){
	  // Ok, let's do a request, first we need a buffer
  //  the buffer must be big enough to hold the data you want to get back
  //  it is also used to hold the request path (no sense wasting SRAM for 
  //  two buffers) before the data is read.  If the data we get back
  //  is larger than the buffer, no problem, you just won't get all the data.
  
  char buffer[250]; // Remember, SRAM is very limited, think carefully about
                    // how big a buffer you really need!

  
  // Now we copy the request path into the buffer.  In this case, the request
  // path is a fixed string, so I store it in FLASH with PSTR and copy it into
  // the buffer with strncpy_P, taking care to leave at least one byte for 
  // the null string terminator.
  memcpy(buffer, 0, sizeof(buffer));     // Ensure the buffer is empty first!
  strncpy_P(buffer, PSTR("/api/v1"), sizeof(buffer)-1);
  
  // For fun, let's add a variable parameter to our fixed request path, first the
  // parameter name itself, which is fixed we will store in FLASH, and then
  // and then our value, in this case the result of millis which we insert
  // into the string with ltoa();
  //strncpy_P(buffer+strlen(buffer), PSTR("&Millis="), sizeof(buffer)-strlen(buffer)-1);
  //ltoa(millis(), buffer+strlen(buffer), 10); // Note "10" is Base10, not a length
    
  // Now we can send the http request and get it's HTTP Response Code If Possible
  //  Common response codes: 200 = OK
  //                         404 = FILE NOT FOUND
  //                         500 = SERVER ERROR
  //
  //  We may also give you a response code below 100, this indicates 
  //     0    = unknown status, but we sent the request OK, so it's probably 200
  //     1-99 = some sort of comms error in making the request
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
    StaticJsonBuffer<250> jsonBuffer;
    JsonObject& res = jsonBuffer.parseObject(buffer);
    float version = res["api_version"];
    const char *status = res["status"];
    debugPrinter->println("API Version");
    debugPrinter->println(version);
    debugPrinter->println("Status");
    debugPrinter->println(status);
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
    
  // * Hostname Note: 
  //
  //  tldr; If you don't provide a hostname, you won't typically get a valid response
  //      code, and you will only be able to contact the default website on a shared IP
  //      but the server will usually 'dumb down' the response for us so it is a bit faster.
  //
  //   Longer Explanation:  
  //
  //   The hostname argument can be set to NULL (or empty string if you like),
  //   if you don't provide it, then we will not send an HTTP version string
  //   with the request.  This will usually make the server behave more "simply"
  //   and not send headers, or status code, it will just dump out the response
  //   body to us, which is nice.  Also, if the IP has more than one website 
  //   attached to it, the request would go to the default website on that IP.
  //
  //   If you do specify the hostname, we will add a version HTTP/1.0 to the 
  //   request.  We can then get headers from this request if you wish.  The 
  //   reason for using HTTP/1.0 instead of 1.1 is that 1.1 introduces 
  //   "chunked" transfer encoding, which would make parsing even slower,
  //   which is not a good thing, giving HTTP/1.0 to an Apache server seems to
  //   force it to avoid chunked encoding with no particular detriment.
}
