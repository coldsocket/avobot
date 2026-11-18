#include "avobot.hpp"


AvoBot avobot;
constexpr int SERIAL_BAUD = 9600;


void setup()
{
	Serial.begin(SERIAL_BAUD);
	avobot.init();
}

void loop()
{
	avobot.update();
	delay(5);
}