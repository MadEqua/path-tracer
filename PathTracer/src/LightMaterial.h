#pragma once

#include "Material.h"

class LightMaterial : public Material
{
public:
	explicit LightMaterial(const Texture *emission, float textureScaleU = 1.0f, float textureScaleV = 1.0f);

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;
};
