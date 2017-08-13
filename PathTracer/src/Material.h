#pragma once

class Ray;
class Vec3;
struct HitRecord;
class Texture;

class Material
{
public:
	Material() : texture(nullptr) {}
	Material(const Texture *texture, float textureScaleU, float textureScaleV) : texture(texture), textureScaleU(textureScaleU), textureScaleV(textureScaleV) {}
	virtual ~Material() = default;
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const = 0;

protected:
	const Texture *texture;
	float textureScaleU, textureScaleV;
};