#include "Utils.h"

#include <glm/glm.hpp>

bool Utils::refract(const glm::vec3 &in, const glm::vec3 &normal, float niOverNt, glm::vec3 &refracted) {
	glm::vec3 uv = glm::normalize(in);

	float dt = glm::dot(uv, normal);
	float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
	if (discriminant > 0.0f) {
		refracted = niOverNt * (uv - normal * dt) - normal * sqrt(discriminant);
		return true;
	}
	return false;
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
