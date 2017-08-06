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
