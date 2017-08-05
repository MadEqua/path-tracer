#pragma once

class Ray;
struct HitRecord;
class AABB;
struct RenderStatistics;

class Hitable
{
public:
	Hitable() {}
	virtual ~Hitable() {}

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const = 0;
	virtual bool boundingBox(float t0, float t1, AABB &aabb) const = 0; //t0 and t1 are animation times (motion blur)
};

