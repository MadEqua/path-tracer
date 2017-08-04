#pragma once

#include "Vec3.h"
#include "Material.h"

class Ray;

class Metal : public Material
{
public:
	Metal(const Vec3 &albedo, float fuzziness);
	virtual ~Metal();

	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const override;

private:
	Vec3 albedo;
	float fuzziness;
};

