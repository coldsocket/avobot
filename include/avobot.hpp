#pragma once

#include "avo-afm.hpp"
#include "avo-radar.hpp"
#include "avo-self.hpp"


class AvoBot
{
	PWMData avopwm = {};

	AvoAFMotors afmotors = {
		.fl_data = {1.00f},
		.fr_data = {1.00f},
		.bl_data = {1.00f},
		.br_data = {1.00f},

		.fl = AOut{&avopwm, 11},
		.fr = AOut{&avopwm,  3},
		.bl = AOut{&avopwm,  5},
		.br = AOut{&avopwm,  6},

		.clock  = DOut{ 4},
		.data   = DOut{ 8},
		.latch  = DOut{12},
		.enable = DOut{ 7}
	};

	AvoRadar radar = {
		.view_pivot = Servo{servo_SG90, AOut{&avopwm, 10}},
		.sonic_sensor = UltraSonic{DOut{A0}, DIn{A1}}
	};

	AvoSelf self = {
	};

public:
	void init () {
		afmotors.init();
		radar.init();
		self.init();
	}

	void update () {
		radar.update(self);
		afmotors.update();
	}
};

// min dist = 15
// max speed = 255
// use serial = Serial
