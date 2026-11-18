#pragma once

#include "hal_uno.hpp"


struct PWMData
{
	float frequency_Hz = 490.20f;
	u32 period_us = 1000000.0f / frequency_Hz;
	u16 tick_max = 255;
	const bool is_active_low = false;

	constexpr PWMData () = default;
	constexpr PWMData (
		float _frequency_Hz,
		u16 _tick_max,
		bool _is_active_low = false
	)
	: frequency_Hz(_frequency_Hz)
	, period_us(1000000.0f / frequency_Hz)
	, tick_max(_tick_max)
	, is_active_low(_is_active_low)
	{}

	/** \param duty_cycle from 0 to 1 */
	template<typename RealTy>
	u16 duty_cycle_to_ticks (RealTy duty_cycle) const {
		const auto raw_ticks = duty_cycle * tick_max;
		const u16 clamped_ticks = clamp<u16>(raw_ticks, 0, tick_max);
		return (!is_active_low)? (clamped_ticks) : (tick_max - clamped_ticks);
	}

	/** \param pulse_len should not exceed period */
	template<typename T>
	u16 pw_us_to_ticks (T pw_us) const {
		const auto raw_ticks = (tick_max * period_us) / pw_us;
		const u16 clamped_ticks = clamp<u16>(raw_ticks, 0, tick_max);
		return (!is_active_low)? (clamped_ticks) : (tick_max - clamped_ticks);
	}

	/** \param duty_cycle from 0 to tick_max */
	template<typename T>
	constexpr float ticks_to_duty_cycle (T ticks) const {
		const float duty_cycle = float(ticks) / tick_max;
		const float clamped_cycle = clamp<float>(duty_cycle, 0, 1);
		return (!is_active_low)? (clamped_cycle) : (1 - clamped_cycle);
	}
};


struct Pin
{
	byte pin;
	constexpr Pin (u8 _pin) : pin(_pin) {};
	constexpr Pin (_AvoUno::PinID _id) : pin(byte(_id)) {};

	template<typename T>
	constexpr Pin& operator = (const T& val) { pin = byte(val); return *this; }

	inline void set_pinmode_in  ()        const { pinMode(pin, INPUT);  }
	inline void set_pinmode_out ()        const { pinMode(pin, OUTPUT); }
	inline void set_pinmode     (int val) const { pinMode(pin, val);    }


	inline void write_digital (u8 val) const {
		digitalWrite(pin, val);
	}
	inline void write_digital_fast_unsafe (u8 val) const {
		_AvoUno::DOUT_MASKS
		.get(_AvoUno::PinID(pin))
		.set_bit(pin, val);
	}
	inline void write_analog (u16 val) const {
		analogWrite(pin, val);
	}
	inline void write_ticks (u16 ticks) const {
		analogWrite(pin, ticks);
	}

	inline int  read_digital      () const {
		return digitalRead(pin);
	}
	inline bool read_digital_fast () const {
		return _AvoUno::DIN_MASKS
		.get(_AvoUno::PinID(pin))
		.get_bit(pin);
	}
	inline int  read_analog () const {
		return analogRead(pin);
	}

	inline u32 pulse_in (u8 state, u32 timeout) const {
		return pulseIn(pin, state, timeout);
	}
};


struct DIn
{
	Pin pin;

	inline void set_pinmode () const { pin.set_pinmode_in(); }
	inline int read_digital () const { return pin.read_digital(); }
	inline int read () const { return read_digital(); }
	inline bool read_digital_fast () const { return pin.read_digital_fast(); }

	inline u32 pulse_in (u8 state, u32 timeout) const {
		return pin.pulse_in(state, timeout);
	}
};

struct DOut
{
	Pin pin;

	inline void set_pinmode () const { pin.set_pinmode_out(); }
	inline void write_digital (u8 val) const { pin.write_digital(val); }
	inline void write (u8 val) const { pin.write_digital(val); }
	inline void write_digital_fast_unsafe (u8 val) const {
		pin.write_digital_fast_unsafe(val);
	}

	inline void pulse_safe_us (u8 val, u16 pulse_us, u16 pull_us = 2)
	{
		constexpr u16 write_call_compensation_us = 2;
		const u16 raw_delay_us = pulse_us - write_call_compensation_us;

		// handle uint underflow
		const u16 delay_us = (raw_delay_us < pulse_us)? raw_delay_us : 0;

		// make sure the initial state is actually the inverse of val
		write(!val);
		delayMicroseconds(pull_us);

		write(val);
		delayMicroseconds(delay_us);
		write(!val);
	}
};


struct AIn
{
	Pin pin;

	inline void set_pinmode () const { pin.set_pinmode_in(); }
	inline int read_analog () const { return pin.read_analog(); }
	inline int read () const { return read_analog(); }

	inline u32 pulse_in (u8 state, u32 timeout) const {
		return pin.pulse_in(state, timeout);
	}
};

struct AOut
{
	PWMData* pwm;
	Pin pin;

	inline void set_pinmode () const { pin.set_pinmode_out(); }
	inline void write_digital (u8 val) const { pin.write_digital(val); }
	inline void write (u8 val) const { pin.write_digital(val); }
	inline void write_digital_fast_unsafe (u8 val) const {
		pin.write_digital_fast_unsafe(val);
	}

	inline void write_ticks (u16 ticks) const { pin.write_analog(ticks); }
	inline void write_analog (u16 ticks) const { write_ticks(ticks); }

	inline void pulse_safe_us (u8 val, u16 us) const
	{
		constexpr u16 pull_for_us = 2;
		constexpr u16 write_call_compensation_us = 2;

		// make sure the initial state is actually inverted
		write_digital(!val);
		delayMicroseconds(pull_for_us);

		write_digital(val);
		delayMicroseconds(us - write_call_compensation_us);
		write_digital(!val);
	}

	/** \param duty_cycle from 0 to 1 */
	template<typename RealTy>
	inline void write_duty_cycle (RealTy duty_cycle) const {
		const auto ticks = pwm->duty_cycle_to_ticks(duty_cycle);
		write_ticks(ticks);
	}
};
