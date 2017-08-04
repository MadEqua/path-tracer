#pragma once

class Ray;
class Vec3;
struct HitRecord;

class Material
{
public:
	Material();
	virtual ~Material();

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;
};

