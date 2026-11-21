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
	DOut disable;

	inline void init ()
	{
		fl      .set_pinmode();
		fr      .set_pinmode();
		bl      .set_pinmode();
		br      .set_pinmode();

		clock   .set_pinmode();
		data    .set_pinmode();
		latch   .set_pinmode();
		disable .set_pinmode();

		disable.write(LOW);
	}

	inline void write_motor_powers () const
	{
		fl.write_duty_cycle(fl_power.get_final_power_abs());
		fr.write_duty_cycle(fr_power.get_final_power_abs());
		bl.write_duty_cycle(bl_power.get_final_power_abs());
		br.write_duty_cycle(br_power.get_final_power_abs());
	}

	// !!! UNORDERED FOR NOW !!!
	inline void write_motor_ins () const
	{
		auto write_dir = [this](const MotorPower& pow, bool invert) {
			const auto dir = pow.get_final_dir() xor invert;
			data.write_digital_fast_unsafe(dir);
			clock.pulse_safe_us(HIGH, 0, 0);
			data.write_digital_fast_unsafe(!dir);
			clock.pulse_safe_us(HIGH, 0, 0);
		};

		write_dir(fl_power, false);
		write_dir(fr_power, false);
		write_dir(bl_power, false);
		write_dir(br_power, false);

		latch.pulse_safe_us(HIGH, 0, 0);
	}

	inline void update ()
	{
		write_motor_powers();
		write_motor_ins();
	}
};