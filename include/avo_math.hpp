#pragma once

#include <math.h>
#include <stdint.h>
#include "vec2.hpp"
#include "fixed_point.hpp"

#undef PI
#undef TAU
#undef RAD_TO_DEG
#undef DEG_TO_RAD
#undef min
#undef max


constexpr float PI = 3.141592653589f;
constexpr float TAU = 2 * PI;
constexpr float RAD_TO_DEG = 360.0f / TAU;
constexpr float DEG_TO_RAD = TAU / 360.0f;


template<typename T>
constexpr T min (T a, T b) { return (a < b)? a : b; }
template<typename T>
constexpr T max (T a, T b) { return (a > b)? a : b; }
template<typename T>
constexpr T signof (T val) { return (val > 0)? 1 : ((val < 0)? -1 : 0); }


template<typename T>
constexpr T clamp (T val, T vmin, T vmax) {
	return min(max(val, vmin), vmax);
}

template<typename T, typename FT>
constexpr T lerp (T start, T target, FT factor) {
	return start + factor * (target - start);
}

template<typename T>
constexpr T map_value (T val, T vmin, T vmax, T out_min, T out_max) {
	return out_min + (out_max - out_min) * (val - vmin) / (vmax - vmin);
}

/** \param vmax inclusive */
template<typename T>
constexpr bool is_within_range (T val, T vmin, T vmax) {
	return (val >= vmin) ? (val <= vmax) : false;
}
