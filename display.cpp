#include <LiquidCrystal_I2C.h>

#include "display.h"

Display::Display()
{
	lcd = new LiquidCrystal_I2C(0x27,20,4);
	lcd->init();
	lcd->backlight();
}

void Display::Processing()
{
	lcd->clear();
	lcd->setCursor(4,0);
  	lcd->print("Toolpass");
  	lcd->setCursor(2,1);
  	lcd->print("Processing...");
}

void Display::Idle()
{
	lcd->clear();
	lcd->setCursor(4,0);
  	lcd->print("Toolpass");
}

void Display::LoggedIn(char *uname, float balance)
{
	lcd->clear();

	lcd->setCursor(0,0);
  	lcd->print("User: ");
	lcd->setCursor(6,0);
  	lcd->print(uname);

  	char balanceStr[7];
	dtostrf(balance, 3, 2, balanceStr);

  	lcd->setCursor(0,1);
  	lcd->print("Balance: $");
  	lcd->setCursor(10,1);
  	lcd->print(balanceStr);
}

// When logged in
// Username
// Current balance
//