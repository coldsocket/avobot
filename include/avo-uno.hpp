#pragma once

#include <Arduino.h>
#include "avo-math.hpp"


#define INT_ROUTINE_PORT_B ISR(PCINT0_vect)
#define INT_ROUTINE_PORT_C ISR(PCINT1_vect)
#define INT_ROUTINE_PORT_D ISR(PCINT2_vect)


struct _AvoUno
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

	enum class PortCIntBit: byte
	{
		// D8-D13
		PORT_B = PCIE0,
		// A0-A5
		PORT_C = PCIE1,
		// D0-D7
		PORT_D = PCIE2,
	};

	enum class PinCIntBit: byte
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


	// 'X' denotes volatility
	struct XMask8Ref
	{
		volatile u8& mask;
		XMask8Ref (volatile u8& _mask) : mask(_mask) {}

		/** \param place 0 inclusive */
		bool get_bit (u8 place) {
			auto bit = (0b1 << place);
			return (mask & bit);
		}

		/** \param place 0 inclusive */
		XMask8Ref& set_bit (u8 place, bool _value) {
			auto remove = (0b1 << place);
			auto value = (_value << place);
			mask = (mask & !remove) | (value);
			return *this;
		}
	};

	struct _DInMasks {
		XMask8Ref PORT_B = PINB;
		XMask8Ref PORT_C = PINC;
		XMask8Ref PORT_D = PIND;

		inline XMask8Ref get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
		}
		inline XMask8Ref get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};
	static _DInMasks DIN_MASKS;

	struct _DOutMasks {
		XMask8Ref PORT_B = PORTB;
		XMask8Ref PORT_C = PORTC;
		XMask8Ref PORT_D = PORTD;

		inline XMask8Ref get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
		}
		inline XMask8Ref get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};
	static _DOutMasks DOUT_MASKS;

	struct _CIntMasks {
		XMask8Ref ENPORT = PCICR;
		XMask8Ref PORT_B = PCMSK0;
		XMask8Ref PORT_C = PCMSK1;
		XMask8Ref PORT_D = PCMSK2;

		inline XMask8Ref get (PortID port_id)
		{
			switch (port_id) {
				case PortID::PORT_B: return PORT_B;
				case PortID::PORT_C: return PORT_C;
				case PortID::PORT_D: return PORT_D;
			};
		}
		inline XMask8Ref get (PinID pin_id) { return get(pinID_to_portID(pin_id)); }
	};
	static _CIntMasks CINT_MASKS;


	static inline PortID pinID_to_portID (PinID pin_id)
	{
		if (is_within_span(pin_id, PinID::D0, PinID::D7))
			return PortID::PORT_D;
		if (is_within_span(pin_id, PinID::D8, PinID::D13))
			return PortID::PORT_B;
		if (is_within_span(pin_id, PinID::A0, PinID::A5))
			return PortID::PORT_C;
	}

	static inline PinBit pinID_to_pinbit (PinID pin_id)
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
	}

	static inline PinCIntBit pinID_to_cintbit (PinID pin_id)
	{
		switch (pin_id) {
			case PinID::D0  : return PinCIntBit::D0;
			case PinID::D1  : return PinCIntBit::D1;
			case PinID::D2  : return PinCIntBit::D2;
			case PinID::D3  : return PinCIntBit::D3;
			case PinID::D4  : return PinCIntBit::D4;
			case PinID::D5  : return PinCIntBit::D5;
			case PinID::D6  : return PinCIntBit::D6;
			case PinID::D7  : return PinCIntBit::D7;
			case PinID::D8  : return PinCIntBit::D8;
			case PinID::D9  : return PinCIntBit::D9;
			case PinID::D10 : return PinCIntBit::D10;
			case PinID::D11 : return PinCIntBit::D11;
			case PinID::D12 : return PinCIntBit::D12;
			case PinID::D13 : return PinCIntBit::D13;
			case PinID::A0  : return PinCIntBit::A0;
			case PinID::A1  : return PinCIntBit::A1;
			case PinID::A2  : return PinCIntBit::A2;
			case PinID::A3  : return PinCIntBit::A3;
			case PinID::A4  : return PinCIntBit::A4;
			case PinID::A5  : return PinCIntBit::A5;
		}
	}

	static inline PortCIntBit portID_to_cintbit (PortID port_id) {
		return (PortCIntBit)port_id;
	}


	static void enable_cint_port (PortID port_id) {
		const auto port_bit = portID_to_cintbit(port_id);
		CINT_MASKS.ENPORT.set_bit(u8(port_bit), true);
	}

	static void enable_cint_pin (PinID pin_id) {
		const auto port_id = pinID_to_portID(pin_id);
		const auto pin_bit = pinID_to_cintbit(pin_id);

		enable_cint_port(port_id);
		auto pinmask = CINT_MASKS.get(port_id);
		pinmask.set_bit(u8(pin_bit), true);
	}

	static inline void enable_cint (PinID pin_id) {
		return enable_cint_pin(pin_id);
	}
};

_AvoUno AvoUno;
