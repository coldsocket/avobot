#pragma once

#include <math.h>
#include <stdint.h>
#include "vec2.hpp"
#include "fixed_point.hpp"


constexpr float AVO_PI = 3.141592653589f;
constexpr float AVO_TAU = 2 * AVO_PI;
constexpr float AVO_RAD_TO_DEG = 360.0F / AVO_TAU;
constexpr float AVO_DEG_TO_RAD = AVO_TAU / 360.0f;


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
	return start + factor * (target - start);
}

template<typename T>
constexpr T map_value (T val, T vmin, T vmax, T out_min, T out_max) {
	return out_min + (val - vmin) * (out_max - out_min) / (vmax - vmin);
}

/** \param vmax inclusive */
template<typename T>
constexpr bool is_within_range (T val, T vmin, T vmax) {
	return (val >= vmin) ? (val <= vmax) : false;
}
