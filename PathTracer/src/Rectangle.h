#pragma once

#include "Object.h"

enum Axis {
	X = 0, Y, Z
};

//Axis aligned rectangle
class Rectangle : public Object
{
public:
	Rectangle(Axis axis, float distToOrigin, Material *material); //Infinite rectangle = plane
	Rectangle(Axis axis, float distToOrigin, float minA, float maxA, float minB, float maxB, Material *material);

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const override;
	virtual bool boundingBox(float t0, float t1, AABB &aabb) const override;

private:
	Axis axis;
	float distToOrigin;
	float minA, maxA;
	float minB, maxB;
};

