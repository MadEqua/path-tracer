#include "Utils.h"

#include <glm/glm.hpp>

bool Utils::refract(const glm::vec3 &in, const glm::vec3 &normal, float indexOfRefraction, glm::vec3 &refracted) {
	float cosi = glm::dot(in, normal);
	float discriminant = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - cosi * cosi);
	if (discriminant > 0.0f) {
		refracted = indexOfRefraction * in - (indexOfRefraction * cosi + sqrt(discriminant)) * normal;
		return true;
	}
	return false; //total internal reflection
}

glm::vec3 Utils::rgbToSrgb(glm::vec3 &rgb) {
	glm::vec3 srgb;
	for (int i = 0; i < 3; ++i) {
		float c = rgb[i];
		if (c <= 0.0031308f) {
			srgb[i] = 12.92f * c;
		}
		else {
			// 0.4166667 = inverse gamma 2.4
			srgb[i] = 1.055f * pow(c, 0.4166667f) - 0.055f;
		}
	}
	return srgb;
}

glm::vec3 Utils::srgbToRgb(glm::vec3 &srgb) {
	glm::vec3 rgb;
	for (int i = 0; i < 3; ++i) {
		float c = srgb[i];
		if (c <= 0.04045f) {
			rgb[i] = c / 12.92f;
		}
		else {
			rgb[i] = pow(((c + 0.055f) / 1.055f), 2.4f);
		}
	}
	return rgb;
}

float Utils::shlick(float cosine, float refractionIdx) {
	float r0 = (1.0f - refractionIdx) / (1.0f + refractionIdx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

glm::vec3 Utils::unpackNormalFromRgb(const glm::vec3 &rgb) {
	return glm::normalize((rgb * 2.0f) - 1.0f);
}

glm::u8vec3 Utils::getPixelFromImage(const byte* image, int w, int h, int x, int y) {
	int invertedY = h - 1 - y;
	const byte* base = image + ((w * invertedY) + x) * 3;
	return glm::u8vec3(*base, *(base + 1), *(base + 2));
}

glm::vec3 Utils::getPixelFromImage(const float* image, int w, int h, int x, int y) {
	int invertedY = h - 1 - y;
	const float* base = image + ((w * invertedY) + x) * 3;
	return glm::vec3(*base, *(base + 1), *(base + 2));
}

void Utils::setPixelToImage(byte *image, int w, int h, int x, int y, const glm::u8vec3 &pixel) {
	int invertedY = h - 1 - y;
	byte* base = image + ((w * invertedY) + x) * 3;
	*base++ = pixel.r;
	*base++ = pixel.g;
	*base = pixel.b;
}

void Utils::setPixelToImage(float *image, int w, int h, int x, int y, const glm::vec3 &pixel) {
	int invertedY = h - 1 - y;
	float* base = image + ((w * invertedY) + x) * 3;
	*base++ = pixel.r;
	*base++ = pixel.g;
	*base = pixel.b;
}
