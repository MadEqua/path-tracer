#pragma once

class Ray;
class Vec3;
struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;
};

