#pragma once

#include "Object.h"

enum Axis {
	X = 0, Y, Z
};

//Rectangle on the Z=0, pointing in the +Z direction. X and Y are limited to [-1, 1] .
//axisToAlign, distToOrigin, rotation, and width/height will be applied to the transform matrix.
class Rectangle : public Object
{
public:
	Rectangle(Axis axis, float distToOrigin, float rotation, float width, float height, Material *material);

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual void computeAABB() const override;

private:
	Axis axis;
};

