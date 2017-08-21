#pragma once

#include "Object.h"

#include "Rectangle.h"
#include <glm/vec3.hpp>

//AABB centered on the origin with [-1, 1] bounds.
//center, dimensions and rotation will be applied to the Transform.
class Box : public Object
{
public:
	Box(const glm::vec3 &center, const glm::vec3 &dimensions, const glm::vec3 &rotation, Material *material);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual void computeAABB() const override;
};
