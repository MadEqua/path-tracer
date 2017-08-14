#include "Mat3.h"

#include "Vec3.h"

Mat3::Mat3() {
	a = 1.0f;
	b = 0.0f;
	c = 0.0f;
	d = 0.0f;
	e = 1.0f;
	f = 0.0f;
	g = 0.0f;
	h = 0.0f;
	i = 1.0f;
}

Mat3::Mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
	values[0] = a;
	values[1] = b;
	values[2] = c;
	values[3] = d;
	values[4] = e;
	values[5] = f;
	values[6] = g;
	values[7] = h;
	values[8] = i;
}

Mat3::Mat3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3) {
	values[0] = col1.x;
	values[1] = col1.y;
	values[2] = col1.z;
	values[3] = col2.x;
	values[4] = col2.y;
	values[5] = col2.z;
	values[6] = col3.x;
	values[7] = col3.y;
	values[8] = col3.z;
}

Mat3 Mat3::operator+(const Mat3 &other) const { 
	return Mat3(a + other.a,
		b + other.b,
		c + other.c,
		d + other.d,
		e + other.e,
		f + other.f,
		g + other.g,
		h + other.h,
		i + other.i);
}


Mat3 Mat3::operator-(const Mat3 &other) const { 
	return Mat3(a - other.a,
		b - other.b,
		c - other.c,
		d - other.d,
		e - other.e,
		f - other.f,
		g - other.g,
		h - other.h,
		i - other.i);
}

Mat3 Mat3::operator*(const Mat3 &other) const {
	Mat3 result;
	result.a = a * other.a + d * other.b + g * other.c;
	result.b = b * other.a + e * other.b + h * other.c;
	result.c = c * other.a + f * other.b + i * other.c;

	result.d = a * other.d + d * other.e + g * other.f;
	result.e = b * other.d + e * other.e + h * other.f;
	result.f = c * other.d + f * other.e + i * other.f;

	result.g = a * other.g + d * other.h + g * other.i;
	result.h = b * other.g + e * other.h + h * other.i;
	result.i = c * other.g + f * other.h + i * other.i;

	return result;
}

Mat3& Mat3::operator+=(const Mat3 &other) {
	a += other.a;
	b += other.b;
	c += other.c;
	d += other.d;
	e += other.e;
	f += other.f;
	g += other.g;
	h += other.h;
	i += other.i;
	return *this;
}

Mat3& Mat3::operator-=(const Mat3 &other) {
	a -= other.a;
	b -= other.b;
	c -= other.c;
	d -= other.d;
	e -= other.e;
	f -= other.f;
	g -= other.g;
	h -= other.h;
	i -= other.i;
	return *this;
}

Mat3& Mat3::operator*=(const Mat3 &other) {
	a = a * other.a + d * other.b + g * other.c;
	b = b * other.a + e * other.b + h * other.c;
	c = c * other.a + f * other.b + i * other.c;

	d = a * other.d + d * other.e + g * other.f;
	e = b * other.d + e * other.e + h * other.f;
	f = c * other.d + f * other.e + i * other.f;

	g = a * other.g + d * other.h + g * other.i;
	h = b * other.g + e * other.h + h * other.i;
	i = c * other.g + f * other.h + i * other.i;
	return *this;
}

Vec3 Mat3::operator*=(const Vec3 &vec) {
	Vec3 result;
	result.x = a * vec.x + d * vec.y + g * vec.z;
	result.y = b * vec.x + e * vec.y + h * vec.z;
	result.z = c * vec.x + f * vec.y + i * vec.z;
	return result;
}

Vec3 operator*(const Mat3 &mat, const Vec3 &vec) {
	Vec3 result;
	result.x = mat.a * vec.x + mat.d * vec.y + mat.g * vec.z;
	result.y = mat.b * vec.x + mat.e * vec.y + mat.h * vec.z;
	result.z = mat.c * vec.x + mat.f * vec.y + mat.i * vec.z;
	return result;
}