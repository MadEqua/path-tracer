#include "Vec3.h"

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {
}

Vec3::Vec3(float v) : x(v), y(v), z(v) {
}

Vec3::Vec3(float x, float y, float z) :
	x(x), y(y), z(z) {
}

void Vec3::normalize() {
	float k = 1.0f / sqrt(x*x + y*y + z*z);
	x *= k;
	y *= k;
	z *= k;
}

void Vec3::clamp(float min, float max) {
	for (int i = 0; i < 3; ++i) {
		if (values[i] < min) values[i] = min;
		else if (values[i] > max) values[i] = max;
	}
}
