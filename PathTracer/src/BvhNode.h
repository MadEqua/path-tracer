#pragma once

#include <vector>

#include "Hitable.h"
#include "AABB.h"
#include "Types.h"

class Object;

struct Comparator {
	Comparator(int axis);
	int axis;
	bool operator() (const Object *i, const Object *j);
};

class BvhNode : public Hitable
{
public:
	BvhNode(std::vector<const Object*> &objects, uint32 l, uint32 r, float t0, float t1);  //l and r are the bounds of the objects list to consider [0, 1]
	~BvhNode();

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual bool boundingBox(float t0, float t1, AABB &aabb) const override;

	const Hitable *left;
	const Hitable *right;

	AABB aabb;
};

