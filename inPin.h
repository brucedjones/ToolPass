#ifndef IN_PIN_h
#define IN_PIN_h

#include <elapsedMillis.h>
#include <SoftwareSerial.h>

class InPin {

public:
	InPin(int pinNum, float logicV, float r1, float r2, float interval, Print *debugPrinter);

	void Poll();

	float GetUsage();

private:
	int pinNum;
	unsigned int positivePolls;
	float logicV, r1, r2, interval;
	elapsedMillis timeElapsed;
	unsigned int prevTime, currTime;
	Print *debugPrinter;


};

#endif