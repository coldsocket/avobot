#pragma once

#include <math.h>
#include "avo_typing.hpp"


template<typename T>
struct Vec2T
{
	T x;
	T y;

	constexpr Vec2T () = default;
	constexpr Vec2T (const T& _x, const T& _y) : x(_x), y(_y) {}

	template<typename OtherT>
	constexpr explicit Vec2T (const Vec2T<OtherT>& other) : x(T(other.x)), y(T(other.y)) {}

	constexpr bool operator == (const Vec2T& other) const { return (x == other.x) && (y == other.y); }
	constexpr bool operator != (const Vec2T& other) const { return (x != other.x) && (y != other.y); }

	constexpr Vec2T& operator += (const Vec2T& other) { x += other.x, y += other.y; return *this; }
	constexpr Vec2T& operator -= (const Vec2T& other) { x -= other.x, y -= other.y; return *this; }
	constexpr Vec2T& operator *= (const Vec2T& other) { x *= other.x, y *= other.y; return *this; }
	constexpr Vec2T& operator /= (const Vec2T& other) { x /= other.x, y /= other.y; return *this; }

	constexpr Vec2T operator + (const Vec2T& other) const { Vec2T v = *this; v += other; return v; }
	constexpr Vec2T operator - (const Vec2T& other) const { Vec2T v = *this; v -= other; return v; }
	constexpr Vec2T operator * (const Vec2T& other) const { Vec2T v = *this; v *= other; return v; }
	constexpr Vec2T operator / (const Vec2T& other) const { Vec2T v = *this; v /= other; return v; }

	constexpr Vec2T& operator *= (const T& other) { return *this *= Vec2T(other, other); }
	constexpr Vec2T& operator /= (const T& other) { return *this /= Vec2T(other, other); }
	constexpr Vec2T  operator *  (const T& other) const { return *this * Vec2T(other, other); }
	constexpr Vec2T  operator /  (const T& other) const { return *this / Vec2T(other, other); }

	friend constexpr Vec2T operator * (const T& scl, const Vec2T& vec) { return Vec2T(scl, scl) * vec; }
	friend constexpr Vec2T operator / (const T& scl, const Vec2T& vec) { return Vec2T(scl, scl) / vec; }

	constexpr Vec2T operator + () const { return *this; }
	constexpr Vec2T operator - () const { return Vec2T(-x, -y); }

	constexpr T length_sq () const { return x*x + y*y; }
	constexpr T length    () const { return sqrt(length_sq()); }
	constexpr T inv_length_sq () const { return T(1) / length_sq(); }
	constexpr T inv_length    () const { return T(1) / length(); }

	constexpr T dot   (const Vec2T& other) const { return x * other.x + y * other.y; }
	constexpr T cross (const Vec2T& other) const { return x * other.y - y * other.x; }
	constexpr T rel_cos (const Vec2T& other) const { return dot  (other) / (length() * other.length()); }
	constexpr T rel_sin (const Vec2T& other) const { return cross(other) / (length() * other.length()); }

	constexpr T angle () const { return atan2(y, x); }
	constexpr T angle_to      (const Vec2T& other) const { return atan2(rel_sin(other), rel_cos(other)); }
	constexpr T angle_to_acos (const Vec2T& other) const { return acos(rel_cos(other)); }
	constexpr T angle_to_asin (const Vec2T& other) const { return asin(rel_sin(other)); }

	static constexpr Vec2T from_angle (const T& angle, const T& len = T(1)) {
		return len * Vec2T(cos(angle), sin(angle));
	}

	constexpr Vec2T normalize () const { return *this / length(); }

	constexpr bool is_finite () const { return isfinite(x) and isfinite(y); }
	constexpr bool is_normalized () const { return length_sq() == T(1); }
};

using Vec2   = Vec2T<double>;
using Vec2f  = Vec2T<float>;

using Vec2i  = Vec2T<int>;
using Vec2zi = Vec2T<isize>;
using Vec2zu = Vec2T<usize>;
