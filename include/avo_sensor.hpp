#pragma once

#include "avo_gpio.hpp"
#include "avo_math.hpp"

// analisis kelangkaan sd air di pulau jawa
struct UltraSonic
{
	DOut trig;
	DIn echo;

	// time before division by 2
	float dt_s = 0;
	float dist_m = 0;
	float dist_offset_m = 0;
	float mach_mps = 343;

	constexpr UltraSonic (DOut _trig, DIn _echo)
	: trig(_trig), echo(_echo)
	{}

	inline void set_pinmode () const {
		trig.set_pinmode();
		echo.set_pinmode();
	}

	inline float get_dist ()
	{
		trig.write(LOW);
		delayMicroseconds(2);

		trig.write(HIGH);
		delayMicroseconds(10);
		trig.write(LOW);

		// get dt with time out so it doesn't stall
		const float dt_us = echo.pulse_in(HIGH, 10000);

		// timed out
		if (dt_us == 0) {
			dt_s = INFINITY;
			dist_m = INFINITY;
			return dist_m;
		}

		dt_s = dt_us * 0.000001;
		const float travel_time_s = dt_s * 0.5f;

		dist_m = travel_time_s * mach_mps + dist_offset_m;
		return dist_m;
	}
};
