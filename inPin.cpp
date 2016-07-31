#include <Arduino.h>
#include <elapsedMillis.h>
#include <SoftwareSerial.h>

#include "inPin.h"

InPin::InPin(int pinNum, float logicV, float r1, float r2, float interval, Print *debugPrinter)
{
	this->debugPrinter = debugPrinter;
	this->pinNum = pinNum;
	this->logicV = logicV;
	this->r1 = r1;
	this->r2 = r2;
	this->interval = interval;
	prevTime = timeElapsed;
	currTime = timeElapsed;
	positivePolls = 0;
}

void InPin::Poll()
{
	currTime = timeElapsed;

	if(currTime-prevTime > interval)
	{

		int value = analogRead(pinNum);
	    float v = (value * logicV) / 1023.0;
	    float v2 = v / (r2 / (r1 + r2));

	    bool state = false;
	    if(v2<=2.5) positivePolls++;
	    prevTime = currTime;
	}
}

float InPin::GetUsage()
{
	float usage = 0.0;

	debugPrinter->println("Reporting usage");
	usage = (positivePolls*(interval)/1000.0);
	positivePolls = 0;

	return usage;
}