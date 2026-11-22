#pragma once

#include "avo_afm.hpp"
#include "avo_sonar.hpp"
#include "avo_self.hpp"


class AvoBot
{
	PWMData avopwm = {};

	AvoMotors afmotors = {
		.fl_power = {1.00f},
		.fr_power = {1.00f},
		.bl_power = {1.00f},
		.br_power = {1.00f},

		.fl = AOut{&avopwm, 11},
		.fr = AOut{&avopwm,  3},
		.bl = AOut{&avopwm,  5},
		.br = AOut{&avopwm,  6},

		.clock   = DOut{ 4},
		.data    = DOut{ 8},
		.latch   = DOut{12},
		.disable = DOut{ 7}
	};

	AvoSonar sonar = {
		.view_pivot = Servo{servo_SG90, AOut{&avopwm, 10}},
		.sonic_sensor = UltraSonic{DOut{A0}, DIn{A1}}
	};

	AvoSelf self = {
	};

public:
	void init () {
		afmotors.init();
		sonar.init();
		self.init();
	}

	void update () {
		sonar.update(self);
		// afmotors.update();
	}
};
