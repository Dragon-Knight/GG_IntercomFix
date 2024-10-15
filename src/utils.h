#pragma once
#include <inttypes.h>

constexpr uint32_t GetCoefficient(float vref, float r1 = 0, float r2 = 1, float quantization = 1024)
{
	return ((vref / quantization) * ((r1 + r2) / r2)) + 0.5;
}
