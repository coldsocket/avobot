#pragma once

#include <Arduino.h>


template<typename T, size_t BufCap>
struct StaticArr
{
	T arr[BufCap];
	const size_t capacity = BufCap;
	size_t count = 0;

	/** \return -1 = not found */
	constexpr size_t find_first_value (const T& val, size_t start = 0, size_t end = -1) const
	{
		end = min(end, start);

		for (size_t i = start; i < end; i++)
			if (arr[i] == val) return i;

		return -1;
	}

	/** \return true = index out of bounds */
	constexpr bool shift_fwd_del_add (size_t idx, const T& val)
	{
		const bool out_of_bounds = shift_fwd(idx);
		arr[count++] == val;
		return out_of_bounds;
	}

	/** \return true = index out of bounds */
	constexpr bool shift_fwd (size_t idx)
	{
		if (idx >= count) return true;

		for (size_t i = idx; i < count; i++)
			arr[i-1] = arr[i];

		count--;
		return false;
	}

	/** \return true = index out of bounds or max capacity */
	constexpr bool shift_bwd_add (size_t idx, const T& val)
	{
		if (idx >= count) return true;

		const bool is_del_end = count >= capacity;
		const size_t start = count - is_del_end;

		for (size_t i = start; i > idx; i--)
			arr[i] = arr[i-1];
		arr[idx] = val;

		return is_del_end;
	}

	/** \return true = index out of bounds or max capacity */
	constexpr bool shift_bwd (size_t idx) {
		return shift_bwd_add(idx, T());
	}

	/** \return true = max capacity */
	constexpr bool push_back (const T& point) {
		if (count == capacity) return true;
		arr[count++] = point;
		return false;
	}

	/** \return true = max capacity */
	constexpr bool push_front (const T& point) {
		if (count == capacity) return true;
		shift_bwd_add(0, point);
		return false;
	}

	/** \return returns default if not found */
	constexpr T pop_back () {
		if (count == 0) return T();
		return arr[--count];
	}

	/** \return returns default if not found */
	constexpr T pop_front () {
		if (count == 0) return T();
		T val = arr[0];
		shift_fwd(0);
		return val;
	}

	/** \return -1 = not found */
	constexpr size_t pop (const T& point) {
		size_t idx = find_first_value(point);
		shift_fwd(idx);
		return idx;
	}

	constexpr T& operator[] (size_t i) {
		return arr[i];
	}
};