#pragma once

#include "AABB.h"

class Ray;
struct HitRecord;
struct RenderStatistics;

class Hitable
{
public:
	virtual ~Hitable() = default;

	//Hitables will assume rays coming in world space, and will return world space data
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const = 0;
	
	inline AABB& getAABB() const { return aabb; }
	
protected:
	mutable AABB aabb;
};

