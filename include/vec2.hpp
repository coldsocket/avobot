#pragma once

#include <math.h>


template<typename T>
struct Vec2T
{
	T x = 0;
	T y = 0;

	constexpr Vec2T () = default;
	constexpr Vec2T (T _v) : x(_v), y(_v) {}
	constexpr Vec2T (T _x, T _y) : x(_x), y(_y) {}

	constexpr bool operator == (const Vec2T& other) const { return (x == other.x) && (y == other.y); }

	constexpr Vec2T& operator += (const Vec2T& other) { x += other.x, y += other.y; return *this; }
	constexpr Vec2T& operator -= (const Vec2T& other) { x -= other.x, y -= other.y; return *this; }
	constexpr Vec2T& operator *= (const Vec2T& other) { x *= other.x, y *= other.y; return *this; }
	constexpr Vec2T& operator /= (const Vec2T& other) { x /= other.x, y /= other.y; return *this; }

	constexpr Vec2T operator + (Vec2T other) const { other += *this; return other; }
	constexpr Vec2T operator - (Vec2T other) const { other -= *this; return other; }
	constexpr Vec2T operator * (Vec2T other) const { other *= *this; return other; }
	constexpr Vec2T operator / (Vec2T other) const { other /= *this; return other; }

	constexpr Vec2T& operator *= (T other) { x *= other, y *= other; return *this; }
	constexpr Vec2T& operator /= (T other) { x /= other, y /= other; return *this; }
	constexpr Vec2T  operator *  (T other) const { auto v = *this; v *= other; return v; }
	constexpr Vec2T  operator /  (T other) const { auto v = *this; v /= other; return v; }

	constexpr Vec2T operator - () const { return { -x, -y }; }

	constexpr T length_sq () const { return x*x + y*y; }
	constexpr T length ()    const { return sqrt(length_sq()); }
	constexpr T inv_length_sq () const { return 1/length_sq(); }
	constexpr T inv_length ()    const { return 1/length(); }

	constexpr T dot  (const Vec2T& other) const { return x * other.x + y * other.y; }
	constexpr T dotP (const Vec2T& other) const { return x * other.y + y * other.x; }
	constexpr T rel_cos (const Vec2T& other) const { return dot (other) / (length() * other.length()); }
	constexpr T rel_sin (const Vec2T& other) const { return dotP(other) / (length() * other.length()); }
	constexpr T angle_acos (const Vec2T& other) const { return acos(rel_cos(other)); }
	constexpr T angle_asin (const Vec2T& other) const { return asin(rel_sin(other)); }

	constexpr Vec2T normalized () const { return Vec2T(x / length(), y / length()); }
	constexpr Vec2T& normalize () { x /= length(), y /= length(); return *this; }

	constexpr bool isfinite () const { return isfinite(x) and isfinite(y); }

	constexpr float get_heading () const { return atan2(y, x); }

	static constexpr Vec2T from_angle (T angle, T magnitude) {
		return { magnitude * cos(angle), magnitude * sin(angle) };
	}
	static constexpr Vec2T from_angle (T angle) {
		return from_angle(angle, 1);
	}
};

using Vec2f = Vec2T<float>;
