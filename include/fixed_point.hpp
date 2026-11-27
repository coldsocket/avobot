#pragma once

#include "avo_typing.hpp"


/**
 * Don't expect this type to be able to store large numbers.
 * 
 * \tparam BinExp determines the ones place by 1 * 2^BinExp, or internally, 1 << BinExp
 * \tparam MulT the type used while doing mul and div operations; Used to avoid over & underflow.
 */
template<typename IntT, size_t BinExp, typename MulT = IntT>
struct fixedT
{
	static_assert(sizeof(IntT) * 8 >= BinExp, "Internal type must be able to fit internal ones value; BinExp is too large.");
	static_assert(sizeof(MulT) >= sizeof(IntT), "Multiplying type cannot be smaller than internal type.");

	IntT intern_val;
	static constexpr size_t bin_exp = BinExp;
	static constexpr IntT one_val = IntT(1) << BinExp;

	constexpr fixedT () = default;
	constexpr fixedT (float  val) : intern_val(IntT(val * one_val)) {}
	constexpr fixedT (double val) : intern_val(IntT(val * one_val)) {}
	explicit constexpr fixedT (const IntT& val) : intern_val(val * one_val) {}

	template<typename T = IntT>
	constexpr T val () const { return T(intern_val) / T(one_val); }

	template<typename OtherIntT, size_t OtherBinExp, typename OtherMulT>
	constexpr explicit fixedT (const fixedT<OtherIntT, OtherBinExp, OtherMulT>& src)
	: intern_val(IntT(f32(one_val) * f32(src.intern_val) / f32(src.one_val)))
	{}

	constexpr auto operator >  (const fixedT& other) const { return intern_val >  other.intern_val; };
	constexpr auto operator <  (const fixedT& other) const { return intern_val <  other.intern_val; };
	constexpr auto operator >= (const fixedT& other) const { return intern_val >= other.intern_val; };
	constexpr auto operator <= (const fixedT& other) const { return intern_val <= other.intern_val; };
	constexpr auto operator == (const fixedT& other) const { return intern_val == other.intern_val; };
	constexpr auto operator != (const fixedT& other) const { return intern_val != other.intern_val; };

	constexpr fixedT& operator += (const fixedT& other) { intern_val += other.intern_val; return *this; }
	constexpr fixedT& operator -= (const fixedT& other) { intern_val -= other.intern_val; return *this; }
	constexpr fixedT& operator *= (const fixedT& other) { intern_val = MulT(intern_val) * MulT(other.intern_val) / MulT(one_val); return *this; }
	constexpr fixedT& operator /= (const fixedT& other) { intern_val = MulT(intern_val) * MulT(one_val) / MulT(other.intern_val); return *this; }

	constexpr fixedT operator + (const fixedT& other) const { fixedT f = *this; f += other; return f; }
	constexpr fixedT operator - (const fixedT& other) const { fixedT f = *this; f -= other; return f; }
	constexpr fixedT operator * (const fixedT& other) const { fixedT f = *this; f *= other; return f; }
	constexpr fixedT operator / (const fixedT& other) const { fixedT f = *this; f /= other; return f; }

	constexpr fixedT& operator += (const IntT& other) { return *this += fixedT(other); }
	constexpr fixedT& operator -= (const IntT& other) { return *this -= fixedT(other); }
	constexpr fixedT& operator *= (const MulT& other) { intern_val *= other; return *this; }
	constexpr fixedT& operator /= (const MulT& other) { intern_val /= other; return *this; }

	constexpr fixedT operator + (const IntT& other) const { fixedT f = *this; f += other; return f; }
	constexpr fixedT operator - (const IntT& other) const { fixedT f = *this; f -= other; return f; }
	constexpr fixedT operator * (const MulT& other) const { fixedT f = *this; f *= other; return f; }
	constexpr fixedT operator / (const MulT& other) const { fixedT f = *this; f /= other; return f; }

	friend constexpr fixedT operator + (const IntT& i, const fixedT& f) { return fixedT(i) + f; }
	friend constexpr fixedT operator - (const IntT& i, const fixedT& f) { return fixedT(i) - f; }
	friend constexpr fixedT operator * (const MulT& i, const fixedT& f) { return f * i; }
	friend constexpr fixedT operator / (const MulT& i, const fixedT& f) { fixedT _; _.intern_val = i * one_val * one_val / f.intern_val; return _; }

	constexpr fixedT operator + () const { return *this; }
	constexpr fixedT operator - () const { fixedT f; f.intern_val = -intern_val; return f; }

	template<typename RealT>
	constexpr fixedT& operator %= (const RealT& other) { intern_val %= other * one_val; return *this; }
	template<typename RealT>
	constexpr fixedT  operator %  (const RealT& other) const { fixedT f = *this; f %= other; return f; }

	constexpr fixedT& operator ++ () { return *this += IntT(1); }
	constexpr fixedT& operator -- () { return *this -= IntT(1); }
	constexpr fixedT  operator ++ (int) { fixedT preinc = *this; ++(*this); return preinc; }
	constexpr fixedT  operator -- (int) { fixedT predec = *this; --(*this); return predec; }
};

using fixed16_5   = fixedT<i16, 5>;
using fixed16_8s  = fixedT<i16, 8, i32>;
using fixed16_10s = fixedT<i16, 10, i32>;

using fixed32_5  = fixedT<i32, 5>;
using fixed32_8  = fixedT<i32, 8>;
using fixed32_10 = fixedT<i32, 10>;
