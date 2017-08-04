#pragma once

#include "Vec3.h"
#include <random>

extern std::default_random_engine rng; //TODO seed?
extern std::uniform_real_distribution<float> dist0to1;
extern std::uniform_real_distribution<float> distNeg1to1;

const float PI = 3.14159265358979f;
const float FLOAT_BIAS = 0.001f;

namespace Utils
{
	Vec3 reflect(const Vec3 &in, const Vec3 &normal);
	bool refract(const Vec3 &in, const Vec3 &normal, float niOverNt, Vec3 &refracted);
	void rgbToSrgb(Vec3 &rgb);
	Vec3 randomInUnitSphere();
	Vec3 randomInUnitDisk();
	float shlick(float cosine, float refractionIdx);
	inline float degToRad(float deg) { return deg * PI / 180.0f; }

	inline float random0To1() { return dist0to1(rng); }
	inline float randomNeg1To1() { return distNeg1to1(rng); }
};