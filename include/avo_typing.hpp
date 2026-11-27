#pragma once

#include <stdint.h>


using u8  = uint8_t;
// using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using usize = size_t;
using isize = int;
static_assert(sizeof(isize) == sizeof(usize), "Signed size type's size doesn't match unsigned size type's size.");

using f32 = float;
static_assert(sizeof(f32) == 4);

// Fails the assert
//
// using f64 = double;
// static_assert(sizeof(f64) == 8);


template<typename T> struct is_integer { static constexpr bool val = (T(0.25) == T(0)) && (T(2) != T(1)); };
template<typename T> struct is_float { static constexpr bool val = (T(0.25) != T(0)) && (T(2) != T(1)); };
template<typename T> struct is_signed { static constexpr bool val = T(-1) < T(1); };

template<typename T>
inline constexpr bool is_integer_v = is_integer<T>::val;
template<typename T>
inline constexpr bool is_float_v = is_float<T>::val;
template<typename T>
inline constexpr bool is_signed_v = is_signed<T>::val;
template<typename T>
inline constexpr bool is_sinteger_v = is_integer<T>::val && is_signed<T>::val;
template<typename T>
inline constexpr bool is_uinteger_v = is_integer<T>::val && !is_signed<T>::val;


// signed bit shift left
template<typename ValT, typename ShfT>
constexpr ValT s_bshift_left (ValT val, ShfT shift)
{
	static_assert(is_uinteger_v<ValT>, "The bit shifting of types other than unsigned ints is not permitted");

	if (!is_signed_v<ValT>)
		return (shift >= 0)? (val << shift) : (val >> -shift);
}
