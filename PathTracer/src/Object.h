#pragma once

#include "Hitable.h"

class Material;

class Object : public Hitable
{
public:
	Object(Material *material) : material(material) {}
	virtual ~Object() {}

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const = 0;
	virtual bool boundingBox(float t0, float t1, AABB &aabb) const = 0;

protected:
	Material *material;
};

