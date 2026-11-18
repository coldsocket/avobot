#pragma once

#include <math.h>


template<typename T>
struct Vec2T
{
	T x = 0;
	T y = 0;

	constexpr Vec2T () = default;
	constexpr Vec2T (T _x, T _y = 0) : x(_x), y(_y) {}

	Vec2T& operator += (const Vec2T& other) { x += other.x, y += other.y; return *this; }
	Vec2T& operator -= (const Vec2T& other) { x -= other.x, y -= other.y; return *this; }
	Vec2T& operator *= (const Vec2T& other) { x *= other.x, y *= other.y; return *this; }
	Vec2T& operator /= (const Vec2T& other) { x /= other.x, y /= other.y; return *this; }

	Vec2T operator + (Vec2T other) const { other += *this; return other; }
	Vec2T operator - (Vec2T other) const { other -= *this; return other; }
	Vec2T operator * (Vec2T other) const { other *= *this; return other; }
	Vec2T operator / (Vec2T other) const { other /= *this; return other; }

	Vec2T& operator *= (T other) { x *= other, y *= other; return *this; }
	Vec2T& operator /= (T other) { x /= other, y /= other; return *this; }
	Vec2T  operator *  (T other) const { auto v = *this; v *= other; return v; }
	Vec2T  operator /  (T other) const { auto v = *this; v /= other; return v; }

	Vec2T operator - () const { return { -x, -y }; }

	T length_sq () const { return x*x + y*y; }
	T length ()    const { return sqrt(length_sq()); }
	T inv_length_sq () const { return 1/length_sq(); }
	T inv_length ()    const { return 1/length(); }

	T dot  (const Vec2T& other) const { return x * other.x + y * other.y; }
	T dotP (const Vec2T& other) const { return x * other.y + y * other.x; }
	T rel_cos (const Vec2T& other) const { return dot (other) / (length()*other.length()); }
	T rel_sin (const Vec2T& other) const { return dotP(other) / (length()*other.length()); }
	T angle_acos (const Vec2T& other) const { return acos(rel_cos(other)); }
	T angle_asin (const Vec2T& other) const { return asin(rel_sin(other)); }

	Vec2T normalized () const { return Vec2T(x / length(), y / length()); }
	Vec2T& normalize () { x /= length(), y /= length(); return *this; }

	bool isfinite () const { return isfinite(x) and isfinite(y); }

	float get_heading () const { return atan2(y, x); }

	static Vec2T from_angle (T angle, T magnitude) {
		return { magnitude * cos(angle), magnitude * sin(angle) };
	}
	static Vec2T from_angle (T angle) {
		return from_angle(angle, 1);
	}
};

using Vec2f = Vec2T<float>;
