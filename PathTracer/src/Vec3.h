#pragma once

#include <iostream>
#include <cmath>

class Vec3
{
public:
	Vec3();
	explicit Vec3(float v);
	Vec3(float x, float y, float z);
	~Vec3();

	union {
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
		float values[3];
	};

	inline float operator[](int i) const { return values[i]; }
	inline float& operator[](int i) { return values[i]; }

	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
	inline Vec3 operator+(const Vec3 &other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
	inline Vec3 operator-(const Vec3 &other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
	inline Vec3 operator*(const Vec3 &other) const { return Vec3(x * other.x, y * other.y, z * other.z); }
	inline Vec3 operator/(const Vec3 &other) const { return Vec3(x / other.x, y / other.y, z / other.z); }

	inline Vec3& operator+=(const Vec3 &other) { x += other.x; y += other.y; z += other.z; return *this; }
	inline Vec3& operator-=(const Vec3 &other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	inline Vec3& operator*=(const Vec3 &other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	inline Vec3& operator/=(const Vec3 &other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	inline Vec3& operator*=(const float v) { x *= v; y *= v; z *= v; return *this; }
	inline Vec3& operator/=(const float v) { x /= v; y /= v; z /= v; return *this; }

	inline void set(float x, float y, float z) { this->x = x; this->y = y; this-> z = z; }
	inline void set(float v) { x = y = z = v; }

	inline float length() const { return sqrt(x*x + y*y + z*z); }
	inline float squaredLength() const { return x*x + y*y + z*z; }
	void normalize();

	inline float dot(const Vec3 &other) const { return x * other.x + y * other.y + z * other.z; }
	inline Vec3 cross(const Vec3 &other) const { return Vec3((y * other.z - z * other.y), (-(x * other.z - z * other.x)), (x * other.y - y * other.x)); }
};

inline Vec3 operator*(const Vec3 &vec, float v) {
	return Vec3(vec.x * v, vec.y * v, vec.z * v);
}

inline Vec3 operator*(float v, const Vec3 &vec) {
	return Vec3(vec.x * v, vec.y * v, vec.z * v);
}

inline Vec3 operator/(const Vec3 &vec, float v) {
	return Vec3(vec.x / v, vec.y / v, vec.z / v);
}

inline std::istream& operator>>(std::istream &is, Vec3 &t) {
	is >> t.x >> t.y >> t.z;
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
	os << "[" << t.x << ", " << t.y << ", " << t.z << "]";
	return os;
}
