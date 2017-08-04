#pragma once

#include "Vec3.h"

class Ray;
class Material;

struct HitRecord {
	float t;
	Vec3 point;
	Vec3 normal;
	Material *material;
};