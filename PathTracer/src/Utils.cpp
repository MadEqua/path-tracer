#include "Utils.h"
#include <cmath>

std::default_random_engine rng;
std::uniform_real_distribution<float> dist0to1(0.0f, 1.0f);
std::uniform_real_distribution<float> distNeg1to1(-1.0f, 1.0f);

Vec3 Utils::reflect(const Vec3 &in, const Vec3 &normal) {
	return in - 2.0f * in.dot(normal) * normal;
}

bool Utils::refract(const Vec3 &in, const Vec3 &normal, float niOverNt, Vec3 &refracted) {
	Vec3 uv = in;
	uv.normalize();

	float dt = uv.dot(normal);
	float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
	if (discriminant > 0.0f) {
		refracted = niOverNt * (uv - normal * dt) - normal * sqrt(discriminant);
		return true;
	}
	return false;
}

void Utils::rgbToSrgb(Vec3 &rgb) {
	for (int i = 0; i < 3; ++i) {
		float c = rgb.values[i];
		if (c <= 0.0031308f) {
			rgb.values[i] = 12.92f * c;
		}
		else {
			// Inverse gamma 2.4
			rgb.values[i] = 1.055f * pow(c, 0.4166667f) - 0.055f;
		}
	}
}

Vec3 Utils::randomInUnitSphere() {
	Vec3 p;
	do {
		p.set(randomNeg1To1(), randomNeg1To1(), randomNeg1To1());
	} while (p.squaredLength() >= 1.0f);
	return p;
}

Vec3 Utils::randomInUnitDisk() {
	Vec3 p;
	do {
		p.set(randomNeg1To1(), randomNeg1To1(), 0);
	} while (p.squaredLength() >= 1.0f);
	return p;
}

float Utils::shlick(float cosine, float refractionIdx) {
	float r0 = (1.0f - refractionIdx) / (1.0f + refractionIdx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}