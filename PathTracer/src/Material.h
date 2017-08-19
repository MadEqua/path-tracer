#pragma once

#include <glm/vec3.hpp>

class Ray;
struct HitRecord;
class Texture;

class Material
{
public:
	Material(bool emissive);
	Material(const Texture *albedo, float textureScaleU, float textureScaleV, bool emissive);
	Material(const Texture *albedo, const Texture *normalMap, float textureScaleU, float textureScaleV, bool emissive);
	virtual ~Material() = default;
	
	virtual bool scatter(const Ray &in, const HitRecord &hitRecord, glm::vec3 &attenuation, Ray &scattered) const = 0;
	virtual glm::vec3 emit(float u, float v) const;

protected:
	const Texture *albedo;
	const Texture *normalMap;
	float textureScaleU, textureScaleV;
	bool emissive;

	glm::vec3 getNormal(const HitRecord &hitRecord) const;
};
