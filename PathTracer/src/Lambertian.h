#pragma once

#include "Material.h"
#include "Vec3.h"

class Lambertian :public Material
{
public:
	explicit Lambertian(const Vec3 &albedo);
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	Vec3 albedo;
};

