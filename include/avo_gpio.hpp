#pragma once

#include "hal_uno.hpp"


struct PWMData
{
	float freq_Hz = 490.20f;
	u32 period_us = 1'000'000.0f / freq_Hz;
	u16 tick_max = 255;
	const bool is_active_low = false;

	constexpr PWMData () = default;
	constexpr PWMData (
		float _frequency_Hz,
		u16 _tick_max,
		bool _is_active_low = false
	)
	: freq_Hz(_frequency_Hz)
	, period_us(1'000'000.0f / freq_Hz)
	, tick_max(_tick_max)
	, is_active_low(_is_active_low)
	{}


	constexpr PWMData& set_frequency (float freq_Hz) {
		freq_Hz = freq_Hz;
		period_us = 1'000'000.0f / freq_Hz;
		return *this;
	}

	constexpr PWMData& set_period (u32 _period_us) {
		period_us = _period_us;
		freq_Hz = 1'000'000.0f / _period_us;
		return *this;
	}


	/** \param duty_cycle from 0 to 1 */
	template<typename RealTy>
	constexpr u16 duty_cycle_to_ticks (RealTy duty_cycle) const {
		const auto raw_ticks = duty_cycle * tick_max;
		const u16 clamped_ticks = clamp<u16>(raw_ticks, 0, tick_max);
		return (!is_active_low)? (clamped_ticks) : (tick_max - clamped_ticks);
	}

	/** \param pw_us should not exceed period_us */
	template<typename T>
	constexpr u16 pw_us_to_ticks (T pw_us) const {
		const auto raw_ticks = (tick_max * period_us) / pw_us;
		const u16 clamped_ticks = clamp<u16>(raw_ticks, 0, tick_max);
		return (!is_active_low)? (clamped_ticks) : (tick_max - clamped_ticks);
	}

	/** \param ticks from 0 to tick_max */
	template<typename T>
	constexpr float ticks_to_duty_cycle (T ticks) const {
		const float duty_cycle = float(ticks) / tick_max;
		const float clamped_cycle = clamp<float>(duty_cycle, 0, 1);
		return (!is_active_low)? (clamped_cycle) : (1 - clamped_cycle);
	}
};


struct Pin
{
	u8 pin;
	constexpr Pin (u8 _pin) : pin(_pin) {};
	constexpr Pin (PinID _id) : pin(u8(_id)) {};

	constexpr Pin& operator = (u8 val) { pin = val; return *this; }
	constexpr Pin& operator = (PinID val) { pin = u8(val); return *this; }

	inline void set_pinmode     (u8 mode) const { pinMode(pin, mode);  }
	inline void set_pinmode_in  ()        const { set_pinmode(INPUT);  }
	inline void set_pinmode_out ()        const { set_pinmode(OUTPUT); }


	inline void write_digital (u8 val) const {
		digitalWrite(pin, val);
	}
	inline void write_digital_fast_unsafe (u8 val) const {
		const PinID id = PinID(pin);
		DOutMasks::get(id).set_bit(id, val);
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
		const PinID id = PinID(pin);
		return DInMasks::get(id).get_bit(id);
	}
	inline int  read_analog () const {
		return analogRead(pin);
	}

	inline u32 pulse_in_us (u8 state, u32 timeout_us) const {
		return pulseIn(pin, state, timeout_us);
	}
};


struct DIn
{
	Pin pin;

	inline void set_pinmode () const { pin.set_pinmode_in(); }
	inline int read_digital () const { return pin.read_digital(); }
	inline int read () const { return read_digital(); }
	inline bool read_digital_fast () const { return pin.read_digital_fast(); }

	inline u32 pulse_in_us (u8 state, u32 timeout_us) const {
		return pin.pulse_in_us(state, timeout_us);
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

	inline void pulse_safe_us (u8 val, u16 pulse_us, u16 pull_us = 2) const
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

	inline u32 pulse_in_us (u8 state, u32 timeout_us) const {
		return pin.pulse_in_us(state, timeout_us);
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

	inline void pulse_safe_us (u8 val, u16 pulse_us, u16 pull_us = 2) const
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

	/** \param duty_cycle from 0 to 1 */
	template<typename RealTy>
	inline void write_duty_cycle (RealTy duty_cycle) const {
		const auto ticks = pwm->duty_cycle_to_ticks(duty_cycle);
		write_ticks(ticks);
	}

	/** \param pw_us should not exceed pwm.period_us */
	inline void write_pw_us (u16 pw_us) const {
		write_ticks(pwm->pw_us_to_ticks(pw_us));
	}
};
