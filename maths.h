#pragma once

#include <stdlib.h>
#include <numeric>

constexpr float PI = 3.141592653589793238462f;
constexpr float INF = std::numeric_limits<float>::infinity();

template<typename T>
inline constexpr T min(T a, T b)
{ return (a < b) ? a : b; }

template<typename T>
inline constexpr T max(T a, T b)
{ return (a > b) ? a : b; }

template<typename T>
inline constexpr T clamp(T x, T min, T max)
{
	if (x < min) x = min;
	else if (x > max) x = max;

	return x;
}

// float to int
inline unsigned int float_to_int(double x)
{
	return (x >= 0) ? (unsigned int)(x + 0.5) : (unsigned int)(x - 0.5);
}

// a wrapper for rand()
inline int rand_int(void)
{ return rand(); }

// rand_float
inline float rand_float(void)
{ return((float)rand() / (float)RAND_MAX); }

// set_rand_seed
inline void set_rand_seed(const int seed)
{ srand(seed); }

// float to byte
inline uint8_t u8fromfloat(float x)
{ return (uint8_t)(x * 255.99f); }

// byte (unsigned char) to float
inline  float u8tofloat(uint8_t x)
{ return (float)(x / 255.99f); }