#pragma once

#include "Material.h"

class Dielectric : public Material
{
public:
	Dielectric(float refractionIndex);
	virtual ~Dielectric();

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	float refractionIndex;
};

