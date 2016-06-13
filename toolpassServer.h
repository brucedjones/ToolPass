#ifndef TOOLPASS_SERVER_h
#define TOOLPASS_SERVER_h

#include "ESP8266_Simple.h"

class ToolpassServer {
	public:
		ToolpassServer(short rxPin, short txPin, const char *SSID, const char *Password, Print *debugPrinter);
		~ToolpassServer();
		void Test();
		bool ToolOn(const char *user_id, int tool_id);
		void ToolOff(const char *user_id, int tool_id);
		void Log(const char *user_id, int tool_id, float seconds, float temperature);
	private:
		ESP8266_Simple *wifi;
		Print *debugPrinter;
};

#endif