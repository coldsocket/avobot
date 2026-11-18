#pragma once

#include "avo_gpio.hpp"
#include "avo_math.hpp"


struct MotorPower
{
	// from 0 to 1
	float power_reduction = 0;
	// from 0 to 1
	float power_factor = 1;
	// from 0 to 1
	float power_raw = 0;

	constexpr MotorPower () = default;
	constexpr MotorPower (float _power_reduction, float _power_factor = 0, float _power_raw = 0)
	: power_reduction(_power_reduction), power_factor(_power_factor), power_raw(_power_raw)
	{}

	constexpr float get_final_power () const {
		return power_raw * (1-power_reduction) * power_factor;
	}
	constexpr float get_final_power_abs () const {
		return abs(power_raw * (1-power_reduction) * power_factor);
	}
	/** \return 0: forwards, 1: backwards */
	constexpr bool get_final_dir () const {
		return signbit(get_final_dir());
	}
};


struct ServoType
{
	float angle_max = 180;
	float turnspeed = 0.8;

	u16 pw_min_us = 1000;
	u16 pw_max_us = 2000;
	u16 pw_span_us = pw_max_us - pw_min_us;

	constexpr ServoType () = default;
	constexpr ServoType (float _angle_max, float _turnspeed, u16 _pw_min, u16 _pw_max)
	: angle_max(_angle_max), turnspeed(_turnspeed), pw_min_us(_pw_min), pw_max_us(_pw_max)
	{}
};

struct Servo
{
	AOut out {nullptr, 0};

	float angle_factor = 1;
	float angle = 0;
	float angle_span = 180;
	float angle_extremum = angle_span/2;
	float turnspeed = 0.8f;

	u16 pw_min_us = 1000;
	u16 pw_max_us = 2000;
	u16 pw_span_us = pw_max_us - pw_min_us;

	bool is_reversed = false;

	constexpr Servo () = default;
	constexpr Servo (
		const ServoType& type, AOut pin,
		float _angle_factor = 1, bool _is_reversed = false
	) : out(pin), angle_factor(_angle_factor), angle_span(type.angle_max)
	  , angle_extremum(angle_span/2), turnspeed(type.turnspeed), pw_min_us(type.pw_min_us)
	  , pw_max_us(type.pw_max_us), pw_span_us(type.pw_span_us), is_reversed(_is_reversed)
	{}

	inline void set_pinmode () const {
		out.set_pinmode();
	}

	constexpr float get_duty_cycle () const
	{
		const auto mul = angle_factor * ( (!is_reversed)? 1 : -1 );
		const auto factored_angle = angle * mul;
		const auto angle_cycle = clamp<float>(
			factored_angle + angle_extremum, 0, angle_span
		);
		return angle_cycle / angle_span;
	}

	inline void write_angle (float new_angle) {
		angle = new_angle;
		out.write_duty_cycle(get_duty_cycle());
	}
};
