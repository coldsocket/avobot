#pragma once

#include "avo_motor.hpp"


struct AvoMotors
{
	MotorPower fl_power;
	MotorPower fr_power;
	MotorPower bl_power;
	MotorPower br_power;

	AOut fl;
	AOut fr;
	AOut bl;
	AOut br;

	DOut clock;
	DOut data;
	DOut latch;
	DOut enable;

	void init ()
	{
		fl     .set_pinmode();
		fr     .set_pinmode();
		bl     .set_pinmode();
		br     .set_pinmode();
		latch  .set_pinmode();
		clock  .set_pinmode();
		enable .set_pinmode();
		data   .set_pinmode();
	}

	void update ()
	{
	}
};