#pragma once

#include <cmath>

#include "Types.h"
#include <glm/glm.hpp>

const float PI = 3.14159265358979f;
const float FLOAT_BIAS = 0.001f;

namespace Utils
{
	bool refract(const glm::vec3 &in, const glm::vec3 &normal, float niOverNt, glm::vec3 &refracted);
	glm::vec3 rgbToSrgb(glm::vec3 &rgb);
	glm::vec3 srgbToRgb(glm::vec3 &srgb);

	float shlick(float cosine, float refractionIdx);
	inline float degToRad(float deg) { return deg * PI / 180.0f; }

	template<class T> inline T min(T a, T b) { return a < b ? a : b; }
	template<class T> inline T max(T a, T b) { return a > b ? a : b; }

	inline float decimalPart(float v) { return v - floor(v); }

	glm::vec3 unpackNormalFromRgb(const glm::vec3 &rgb);

	glm::vec3 getPixelFromImage(const float* image, int w, int h, int x, int y);
	glm::u8vec3 getPixelFromImage(const byte* image, int w, int h, int x, int y);
	
	void setPixelToImage(float* image, int w, int h, int x, int y, const glm::vec3 &pixel);
	void setPixelToImage(byte* image, int w, int h, int x, int y, const glm::u8vec3 &pixel);

	inline float getLuminanceFromRgb(const glm::vec3 &color) { return glm::dot(glm::vec3(0.2126f, 0.7152f, 0.0722f), color); }
};
