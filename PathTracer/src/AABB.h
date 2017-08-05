#pragma once

#include "Vec3.h"

class Ray;

class AABB
{
public:
	AABB();
	AABB(const Vec3 &min, const Vec3 &max);
	~AABB();

	inline void set(const Vec3 &min, const Vec3 &max) { this->min = min; this->max = max; }
	void enclose(const AABB &aabb);

	bool hit(const Ray &ray, float tMin, float tMax) const;

	Vec3 min, max;
};

