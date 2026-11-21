#pragma once

#include "avo_gpio.hpp"
#include "avo_math.hpp"


struct UltraSonic
{
	DOut trig;
	DIn echo;

	// time before division by 2
	float dt_s = 0;
	float dist_m = 0;
	float dist_offset_m = 0;
	float mach_mps = 343;


	static constexpr u32 pulse_in_timeout_us = 10'000;


	constexpr UltraSonic (DOut _trig, DIn _echo)
	: trig(_trig), echo(_echo)
	{}

	inline void set_pinmode () const {
		trig.set_pinmode();
		echo.set_pinmode();
	}

	inline float get_dist ()
	{
		trig.pulse_safe_us(HIGH, 10, 1);

		// get dt with time out so it doesn't stall
		const float dt_us = echo.pulse_in_us(HIGH, pulse_in_timeout_us);

		// timed out
		if (dt_us == 0) {
			dt_s = INFINITY;
			dist_m = INFINITY;
			return dist_m;
		}

		dt_s = dt_us * 0.00'000'1;
		const float travel_time_s = dt_s * 0.5f;

		dist_m = travel_time_s * mach_mps + dist_offset_m;
		return dist_m;
	}
};
