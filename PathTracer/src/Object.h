#pragma once

#include "Hitable.h"
#include "Transform.h"
#include "AABB.h"

class Material;

//Representing a renderable object on a scene
class Object : public Hitable
{
public:
	explicit Object(const Material *material) : material(material) {}

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const = 0;

	//Compute AABB on world space
	virtual void computeAABB() const = 0;
	inline void computeTransformMatrices() const { transform.computeMatrices(); };

	inline Transform& getTransform() const { return transform; }

protected:
	const Material *material;
	mutable Transform transform;
};
