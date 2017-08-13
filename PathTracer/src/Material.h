#pragma once

class Ray;
class Vec3;
struct HitRecord;
class Texture;

class Material
{
public:
	Material() : albedo(nullptr) {}
	Material(const Texture *albedo, float textureScaleU, float textureScaleV) : albedo(albedo), textureScaleU(textureScaleU), textureScaleV(textureScaleV) {}
	virtual ~Material() = default;
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;

protected:
	const Texture *albedo;
	float textureScaleU, textureScaleV;
};