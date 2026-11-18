#pragma once

#include <Arduino.h>
#include "avo_math.hpp"


#define PCINT_ROUTINE_PORT_B ISR(PCINT0_vect)
#define PCINT_ROUTINE_PORT_C ISR(PCINT1_vect)
#define PCINT_ROUTINE_PORT_D ISR(PCINT2_vect)


namespace hal
{
	enum class PortID: byte
	{
		// D8-D13
		PORT_B = 0,
		// A0-A5
		PORT_C = 1,
		// D0-D7
		PORT_D = 2,
	};

	enum class PinID: byte
	{
		/* PORT D */
		D0  = 0,  D1  = 1,  D2  = 2,
		/* PORT D */
		D3  = 3,  D4  = 4,  D5  = 5,
		/* PORT D */
		D6  = 6,  D7  = 7,

		/* PORT B */
		D8  = 8,  D9  = 9,  D10 = 10,
		/* PORT B */
		D11 = 11, D12 = 12, D13 = 13,

		/* PORT C */
		A0  = 14, A1  = 15, A2  = 16,
		/* PORT C */
		A3  = 17, A4  = 18, A5  = 19
	};

	enum class PinBit: byte
	{
		/* PORT D */
		D0  = PD0, D1  = PD1, D2  = PD2,
		/* PORT D */
		D3  = PD3, D4  = PD4, D5  = PD5,
		/* PORT D */
		D6  = PD6, D7  = PD7,

		/* PORT B */
		D8  = PB0, D9  = PB1, D10 = PB2,
		/* PORT B */
		D11 = PB3, D12 = PB4, D13 = PB5,

		/* PORT C */
		A0  = PC0, A1  = PC1, A2  = PC2,
		/* PORT C */
		A3  = PC3, A4  = PC4, A5  = PC5
	};

	enum class PortPCIntBit: byte
	{
		// D8-D13
		PORT_B = PCIE0,
		// A0-A5
		PORT_C = PCIE1,
		// D0-D7
		PORT_D = PCIE2,
	};

	enum class PinPCIntBit: byte
	{
		/* PORT D */
		D0  = PCINT16, D1  = PCINT17, D2  = PCINT18,
		/* PORT D */
		D3  = PCINT19, D4  = PCINT20, D5  = PCINT21,
		/* PORT D */
		D6  = PCINT22, D7  = PCINT23,

		/* PORT B */
		D8  = PCINT0,  D9  = PCINT1,  D10 = PCINT2,
		/* PORT B */
		D11 = PCINT3,  D12 = PCINT4,  D13 = PCINT5,

		/* PORT C */
		A0  = PCINT8,  A1  = PCINT9,  A2  = PCINT10,
		/* PORT C */
		A3  = PCINT11, A4  = PCINT12, A5  = PCINT13
	};


	static constexpr PortID pinID_to_portID (PinID pin_id)
	{
		if (is_within_span(pin_id, PinID::D0, PinID::D7))
			return PortID::PORT_D;
		if (is_within_span(pin_id, PinID::D8, PinID::D13))
			return PortID::PORT_B;
		if (is_within_span(pin_id, PinID::A0, PinID::A5))
			return PortID::PORT_C;
		return PortID::PORT_B;
	}

	static constexpr PinBit pinID_to_pinbit (PinID pin_id)
	{
		switch (pin_id) {
			case PinID::D0  : return PinBit::D0;
			case PinID::D1  : return PinBit::D1;
			case PinID::D2  : return PinBit::D2;
			case PinID::D3  : return PinBit::D3;
			case PinID::D4  : return PinBit::D4;
			case PinID::D5  : return PinBit::D5;
			case PinID::D6  : return PinBit::D6;
			case PinID::D7  : return PinBit::D7;
			case PinID::D8  : return PinBit::D8;
			case PinID::D9  : return PinBit::D9;
			case PinID::D10 : return PinBit::D10;
			case PinID::D11 : return PinBit::D11;
			case PinID::D12 : return PinBit::D12;
			case PinID::D13 : return PinBit::D13;
			case PinID::A0  : return PinBit::A0;
			case PinID::A1  : return PinBit::A1;
			case PinID::A2  : return PinBit::A2;
			case PinID::A3  : return PinBit::A3;
			case PinID::A4  : return PinBit::A4;
			case PinID::A5  : return PinBit::A5;
		}
		return PinBit::D0;
	}

	static constexpr PinPCIntBit pinID_to_pcintbit (PinID pin_id)
	{
		switch (pin_id) {
			case PinID::D0  : return PinPCIntBit::D0;
			case PinID::D1  : return PinPCIntBit::D1;
			case PinID::D2  : return PinPCIntBit::D2;
			case PinID::D3  : return PinPCIntBit::D3;
			case PinID::D4  : return PinPCIntBit::D4;
			case PinID::D5  : return PinPCIntBit::D5;
			case PinID::D6  : return PinPCIntBit::D6;
			case PinID::D7  : return PinPCIntBit::D7;
			case PinID::D8  : return PinPCIntBit::D8;
			case PinID::D9  : return PinPCIntBit::D9;
			case PinID::D10 : return PinPCIntBit::D10;
			case PinID::D11 : return PinPCIntBit::D11;
			case PinID::D12 : return PinPCIntBit::D12;
			case PinID::D13 : return PinPCIntBit::D13;
			case PinID::A0  : return PinPCIntBit::A0;
			case PinID::A1  : return PinPCIntBit::A1;
			case PinID::A2  : return PinPCIntBit::A2;
			case PinID::A3  : return PinPCIntBit::A3;
			case PinID::A4  : return PinPCIntBit::A4;
			case PinID::A5  : return PinPCIntBit::A5;
		}
		return PinPCIntBit::D0;
	}

	static constexpr PortPCIntBit portID_to_pcintbit (PortID port_id) {
		return (PortPCIntBit)port_id;
	}


	template<typename T>
	struct RegPtr
	{
		volatile T* data;
		constexpr RegPtr (volatile T& ref) : data(&ref) {}
		constexpr RegPtr (volatile T* ptr) : data( ptr) {}

		/** \param place 0 inclusive */
		constexpr bool get_bit (u8 place) const {
			return *data & (0b1 << place);
		}
		constexpr bool get_bit (PinID pin_id) const {
			return get_bit((u8)pinID_to_pinbit(pin_id));
		}
		constexpr T get_val () const {
			return *data;
		}

		/** \param place 0 inclusive */
		constexpr const RegPtr& set_bit (u8 place, bool val) const {
			const auto remove = (0b1 << place);
			const auto value = (val << place);
			*data = (*data & ~remove) | (value);
			return *this;
		}
		/** \param place 0 inclusive */
		constexpr const RegPtr& set_bit (PinID pin_id, bool val) const {
			return set_bit((u8)pinID_to_pinbit(pin_id), val);
		}
		constexpr const RegPtr& set_val (const T& val) const {
			*data = val;
			return *this;
		}
	};
	using RegPtr8 = RegPtr<u8>;


	struct DataDirMasks {
		static inline const RegPtr8 PORT_B = PORTB;
		static inline const RegPtr8 PORT_C = PORTC;
		static inline const RegPtr8 PORT_D = PORTD;

		static inline RegPtr8 get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
			return PORT_B;
		}
		static inline RegPtr8 get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};

	struct DInMasks {
		static inline const RegPtr8 PORT_B = PORTB;
		static inline const RegPtr8 PORT_C = PORTC;
		static inline const RegPtr8 PORT_D = PORTD;

		static inline RegPtr8 get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
			return PORT_B;
		}
		static inline RegPtr8 get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};

	struct DOutMasks {
		static inline const RegPtr8 PORT_B = PORTB;
		static inline const RegPtr8 PORT_C = PORTC;
		static inline const RegPtr8 PORT_D = PORTD;

		static inline RegPtr8 get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
			return PORT_B;
		}
		static inline RegPtr8 get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};

	struct PCIntMasks {
		static inline const RegPtr8 ENPORT = PCICR;
		static inline const RegPtr8 PORT_B = PORTB;
		static inline const RegPtr8 PORT_C = PORTC;
		static inline const RegPtr8 PORT_D = PORTD;

		static inline RegPtr8 get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
			return PORT_B;
		}
		static inline RegPtr8 get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};


	static inline void enable_pcint_port (PortID port_id) {
		const auto port_bit = portID_to_pcintbit(port_id);
		PCIntMasks::ENPORT.set_bit((u8)port_bit, true);
	}

	static inline void enable_pcint_pin (PinID pin_id) {
		const auto port_id = pinID_to_portID(pin_id);
		const auto pin_bit = pinID_to_pcintbit(pin_id);

		enable_pcint_port(port_id);
		auto pinmask = PCIntMasks::get(port_id);
		pinmask.set_bit(u8(pin_bit), true);
	}

	static inline void enable_pcint (PinID pin_id) {
		return enable_pcint_pin(pin_id);
	}
}

// why not
using namespace hal;
