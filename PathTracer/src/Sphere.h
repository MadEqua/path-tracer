#pragma once

#include "Object.h"
#include <glm/vec3.hpp>

//Unit sphere centered on origin. Center and radius will be applied to the tranform matrix.
class Sphere : public Object
{
public:
	Sphere(const glm::vec3 &center, float radius, Material *material);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual void computeAABB() const override;
	
private:
	void computeHitRecord(const Ray &worldRay, const Ray &localRay, float t, HitRecord &hitRecord) const;
};

