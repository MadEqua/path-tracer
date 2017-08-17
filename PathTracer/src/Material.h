#pragma once

#include "Vec3.h"

class Ray;
struct HitRecord;
class Texture;

class Material
{
public:
	Material();
	Material(const Texture *albedo, float textureScaleU, float textureScaleV, bool emissive);
	Material(const Texture *albedo, const Texture *normalMap, float textureScaleU, float textureScaleV, bool emissive);
	virtual ~Material() = default;
	
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;
	virtual Vec3 emit(float u, float v) const;

protected:
	const Texture *albedo;
	const Texture *normalMap;
	float textureScaleU, textureScaleV;
	bool emissive;

	Vec3 getNormal(const HitRecord &hitRecord) const;
};
