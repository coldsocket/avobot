#pragma once

#include<Arduino.h>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using usize = size_t;
using isize = int;
static_assert(sizeof(isize) == sizeof(usize), "Signed size type's size doesn't match unsigned size type's size.");

using f32 = float;
static_assert(sizeof(f32) == 4);
using f64 = double;
static_assert(sizeof(f64) == 4);
