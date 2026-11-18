#pragma once

#include <Arduino.h>
#include "avo-math.hpp"


struct AvoSelf
{
	Vec2f pos = {0, 0};
	float heading = 0;

	void init () { auto& self = *this;
		self.__repr__();
	}

	static inline const char* __repr__ ()
	{
		static constexpr char self__repr__[] = "I think, therefore I AM.";
		Serial.println(self__repr__);
		return self__repr__;
	}
};