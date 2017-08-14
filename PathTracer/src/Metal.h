#pragma once

#include "Vec3.h"
#include "Material.h"

class Ray;

class Metal : public Material
{
public:
	Metal(const Texture *albedo, float fuzziness, float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	Metal(const Texture *albedo, const Texture *normalMap, float fuzziness, float textureScaleU = 1.0f, float textureScaleV = 1.0f);
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	float fuzziness;
};

