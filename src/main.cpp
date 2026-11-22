#include "avobot.hpp"


AvoBot avobot;
constexpr u32 SERIAL_BAUD = 115'200;


void setup()
{
	Serial.begin(SERIAL_BAUD);
	avobot.init();
}

void loop()
{
	avobot.update();
	delay(100);
}