#pragma once

class Ray;
class Material;
struct HitRecord;

class Object
{
public:
	Object(Material *material);
	virtual ~Object();

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const = 0;

protected:
	Material *material;
};

