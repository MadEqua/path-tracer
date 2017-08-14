#pragma once

#include "Vec3.h"

class Material;

struct HitRecord {
	float t;
	Vec3 point;
	Vec3 tangent;
	Vec3 bitangent;
	Vec3 normal;
	float u, v; //Texture coordinates
	Material *material;
};