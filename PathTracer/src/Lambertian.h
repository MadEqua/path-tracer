#pragma once

#include "Material.h"
#include "Vec3.h"

class Lambertian :public Material
{
public:
	Lambertian(const Vec3 &albedo);
	virtual ~Lambertian();

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	Vec3 albedo;
};

