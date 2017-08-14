#pragma once

#include "Material.h"

class Dielectric : public Material
{
public:
	explicit Dielectric(float refractionIndex);
	Dielectric(const Texture *albedo, float refractionIndex, float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	Dielectric(const Texture *albedo, const Texture *normalMap, float refractionIndex, float textureScaleU = 1.0f, float textureScaleV = 1.0f);

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	float refractionIndex;
};

