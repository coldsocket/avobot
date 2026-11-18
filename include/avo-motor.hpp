#pragma once

#include "avo-gpio.hpp"
#include "avo-math.hpp"


struct MotorPowerData
{
	// from 0 to 1
	float power_reduction = 0;
	// from 0 to 1
	float power_factor = 1;
	// from 0 to 1
	float power_raw = 0;

	constexpr MotorPowerData () = default;
	constexpr MotorPowerData (float _power_factor, float _power_reduction = 0, float _power_raw = 0)
	: power_factor(_power_factor), power_reduction(_power_reduction), power_raw(_power_raw)
	{}

	float get_final_power () const {
		return power_raw * (1-power_reduction) * power_factor;
	}
};


struct ServoType
{
	float angle_span = 180;
	float angle_extremum = angle_span/2;
	float turnspeed = 0.8;

	constexpr ServoType () = default;
	constexpr ServoType (float _angle_span, float _turnspeed)
	: angle_span(_angle_span), angle_extremum(angle_span/2), turnspeed(_turnspeed)
	{}
};

struct Servo
{
	AOut out {nullptr, 0};

	float angle_factor = 1;
	float angle = 0;
	float duty_cycle = 0;

	float angle_span = 180;
	float angle_extremum = angle_span/2;
	float turnspeed = 0.8f;

	bool is_reversed = false;

	inline Servo (
		const ServoType& type, AOut pin,
		float _angle_factor = 1, bool _is_reversed = false
	) : out(pin), angle_factor(_angle_factor), angle_span(type.angle_span)
	  , angle_extremum(type.angle_extremum), turnspeed(type.turnspeed), is_reversed(_is_reversed)
	{}

	inline Servo (
		AOut pin, float _angle_span, float _turnspeed,
		float _angle_factor = 1, bool _is_reversed = false
	) : out(pin), angle_factor(_angle_factor), angle_span(_angle_span)
	  , angle_extremum(_angle_span/2), turnspeed(_turnspeed), is_reversed(_is_reversed)
	{}

	inline void set_pinmode () {
		out.set_pinmode();
	}

	inline void update_duty_cycle ()
	{
		const auto mul = angle_factor * ( (!is_reversed)? 1 : -1 );
		const auto factored_angle = angle * mul;
		const auto angle_cycle = clamp<float>(
			factored_angle + angle_extremum, 0, angle_span
		);
		duty_cycle = angle_cycle / angle_span;
	}

	inline void write_angle (float new_angle) {
		angle = new_angle;
		update_duty_cycle();
		out.write_duty_cycle(duty_cycle);
	}
};
