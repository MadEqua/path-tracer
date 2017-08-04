#pragma once

#include "Object.h"
#include "Vec3.h"

class Sphere : public Object
{
private:
	Vec3 center;
	float radius;

public:
	Sphere(const Vec3 &center, float radius, Material *material);
	virtual ~Sphere();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord &hitRecord) const override;
};

