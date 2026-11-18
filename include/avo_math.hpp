#pragma once

#include <math.h>
#include <stdint.h>
#include "vec2.hpp"


template<typename T>
constexpr T avo_min (T a, T b) { return (a < b)? a : b; }
template<typename T>
constexpr T avo_max (T a, T b) { return (a > b)? a : b; }
template<typename T>
constexpr T signof (T val) { return (val > 0)? 1 : ((val < 0)? -1 : 0); }


template<typename T>
constexpr T clamp (T val, T vmin, T vmax) {
	return avo_min(avo_max(val, vmin), vmax);
}

template<typename T, typename FT>
constexpr T lerp (T start, T target, FT factor) {
	return start + (target - start) * factor;
}

template<typename T>
constexpr T map_value (T val, T vmin, T vmax, T out_min, T out_max) {
	return (val - vmin) * (out_max - out_min) / (vmax - vmin) + out_min;
}

template<typename T>
constexpr bool is_within_span (T val, T min, T max) {
	return (val >= min) ? (val <= max) : false;
}
