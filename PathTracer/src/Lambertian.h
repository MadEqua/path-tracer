#pragma once

#include "Material.h"

class Lambertian :public Material
{
public:
	explicit Lambertian(const Texture *texture, float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;
};
