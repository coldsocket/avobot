#pragma once

#include <Arduino.h>


template<typename T, size_t BufCap>
struct StaticArr
{
	T arr[BufCap];
	const size_t capacity = BufCap;
	size_t count = 0;

	/** \return -1 = not found */
	constexpr size_t find_first_value (const T& val, size_t start = 0) const {
		for (size_t i = start; i < count; i++)
			if (arr[i] == val) return i;
		return -1;
	}

	/** \return true = index out of bounds */
	constexpr bool shift_fwd_delete_add (size_t idx, const T& val)
	{
		if (idx >= count) return true;

		for (size_t i = idx; i < count-1; i++)
			arr[i] = arr[i+1];

		arr[count-1] = val;

		return false;
	}

	/** \return true = index out of bounds */
	constexpr bool shift_fwd_delete (size_t idx) {
		return shift_fwd_delete_add(idx, T());
	}

	/** \return true = index out of bounds or max capacity */
	constexpr bool shift_bwd_add (size_t idx, const T& val)
	{
		if (idx >= count) return true;

		arr[idx] = val;

		for (size_t i = idx; i < count-1; i++)
			arr[i+1] = arr[i];

		if (count >= capacity) return true;

		arr[count] = arr[count-1];
		count++;
		return false;
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
		shift_fwd_delete(0);
		return val;
	}

	/** \return -1 = not found */
	constexpr size_t pop (const T& point) {
		size_t idx = find_first_value(point);
		shift_fwd_delete(idx);
		return idx;
	}

	constexpr T& operator[] (size_t i) {
		return arr[i];
	}
};