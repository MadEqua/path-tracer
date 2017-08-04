#pragma once

#include "Vec3.h"

class Ray 
{
public:
	Ray();
	Ray(const Vec3 &origin, const Vec3 &direction);
	~Ray();

	Vec3 origin;
	Vec3 direction;

	inline void set(const Vec3 &origin, const Vec3 &direction) { this->origin = origin; this->direction = direction; }
	inline Vec3 pointAtParameter(float t) const { return origin + t * direction; }
};