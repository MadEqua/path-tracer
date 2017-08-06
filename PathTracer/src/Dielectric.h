#pragma once

#include "Material.h"

class Dielectric : public Material
{
public:
	explicit Dielectric(float refractionIndex);
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	float refractionIndex;
};

