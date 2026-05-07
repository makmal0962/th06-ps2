#pragma once


#include <SDL.h>
#include <stdio.h>
#include <cstdint>

#ifdef __PS2__
#include <tamtypes.h>
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef unsigned long long u64;
typedef unsigned int iptr;
#else
typedef std::int8_t i8;
typedef std::uint8_t u8;
typedef std::int16_t i16;
typedef std::uint16_t u16;
typedef std::int32_t i32;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::intptr_t iptr;
#endif
typedef float f32;
typedef double f64;

#ifdef __ANDROID__
inline f32 uf32(const f32* ptr) {
    u32 temp = *(const u32*)ptr;
    __asm__ volatile ("":"+r"(temp));
    return __builtin_bit_cast(float, temp);
}
#else
inline f32 uf32(const f32* ptr) {
    return *ptr;
}
#endif