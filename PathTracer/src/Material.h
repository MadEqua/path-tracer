#pragma once

#include "Vec3.h"

class Ray;
struct HitRecord;
class Texture;

class Material
{
public:
	Material() : albedo(nullptr), normalMap(nullptr) {}
	Material(const Texture *albedo, float textureScaleU, float textureScaleV) : albedo(albedo), normalMap(nullptr), textureScaleU(textureScaleU), textureScaleV(textureScaleV) {}
	Material(const Texture *albedo, const Texture *normalMap, float textureScaleU, float textureScaleV) : albedo(albedo), normalMap(normalMap), textureScaleU(textureScaleU), textureScaleV(textureScaleV) {}
	virtual ~Material() = default;
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;

protected:
	const Texture *albedo;
	const Texture *normalMap;
	float textureScaleU, textureScaleV;

	Vec3 getNormal(const HitRecord &hitRecord) const;
};