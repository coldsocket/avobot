#pragma once

#include "isize.hpp"


/**
 * \tparam BinExp is the factor that determines the decimal point's
 * (or in this case binary point) place by intern_val * 2^BinExp
 */
template<typename IntT, isize BinExp>
struct FixedT
{
	IntT intern_val;
	static constexpr isize bin_exp = BinExp;
};