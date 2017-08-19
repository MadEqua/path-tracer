#pragma once

#include <vector>

#include "Hitable.h"
#include "AABB.h"
#include "Types.h"

class Object;

struct Comparator {
	explicit Comparator(int axis);
	int axis;
	bool operator() (const Object *i, const Object *j);
};

class BvhNode : public Hitable
{
public:
	BvhNode(std::vector<const Object*> &objects, uint32 l, uint32 r);  //l and r are the bounds of the objects list to consider [0, 1]
	~BvhNode();

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;

	const Hitable *left;
	const Hitable *right;
};

