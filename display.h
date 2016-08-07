#ifndef DISPLAY_h
#define DISPLAY_h

#include <LiquidCrystal_I2C.h>

class Display {
public:
	Display();

	void Processing();

	void Idle();

	void LoggedIn(char *uname, float balance);

private:
	LiquidCrystal_I2C *lcd;
};

#endif