#include <Arduino.h>
#include "avobot.hpp"


AvoBot avobot;
constexpr bool DO_SERIAL = true;
constexpr int SERIAL_BAUD = 9600;


void setup()
{
	if (DO_SERIAL)
		Serial.begin(SERIAL_BAUD);

	avobot.init();
}

void loop()
{
	avobot.update();
	delay(10);
}