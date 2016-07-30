#ifndef TOOLPASS_SERVER_h
#define TOOLPASS_SERVER_h

#include "ESP8266_Simple.h"
#include <SoftwareSerial.h>

class ToolpassServer {
	public:
		ToolpassServer(short rxPin, short txPin, const char *SSID, const char *Password, Print *debugPrinter, int tool_id);
		~ToolpassServer();
		void Test();
		bool ToolOn(char *user_id);
		void ToolOff();
		void Log(float seconds, float temperature);
	private:
		ESP8266_Simple *wifi;
		Print *debugPrinter;
		char *user_id;
		int tool_id;
};

#endif