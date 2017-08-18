#pragma once

#include "Material.h"

class Lambertian :public Material
{
public:
	explicit Lambertian(const Texture *albedo,  float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	Lambertian(const Texture *albedo, const Texture *normalMap, float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, glm::vec3 &attenuation, Ray &scattered) const override;
};
