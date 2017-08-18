#pragma once

#include "Object.h"
#include <glm/vec3.hpp>

class Sphere : public Object
{
private:
	glm::vec3 center;
	float radius;

public:
	Sphere(const glm::vec3 &center, float radius, Material *material);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual bool boundingBox(float t0, float t1, AABB &aabb) const override;

private:
	void computeHitRecord(const Ray &ray, float t, HitRecord &hitRecord) const;
};

