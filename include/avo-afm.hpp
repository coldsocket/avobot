#pragma once

#include "avo-motor.hpp"


struct AvoAFMotors
{
	MotorPowerData fl_data;
	MotorPowerData fr_data;
	MotorPowerData bl_data;
	MotorPowerData br_data;

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