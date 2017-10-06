#include "Lambertian.h"

#include <glm/gtc/random.hpp>

#include "Utils.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Texture.h"

Lambertian::Lambertian(const Texture *texture, float textureScaleU, float textureScaleV) :
	Material(texture, textureScaleU, textureScaleV, false) {
}

Lambertian::Lambertian(const Texture *texture, const Texture *normalMap, float textureScaleU, float textureScaleV) :
	Material(texture, normalMap, textureScaleU, textureScaleV, false) {
}

bool Lambertian::scatter(const Ray &in, const HitRecord &hitRecord, glm::vec3 &attenuation, Ray &scattered) const {
	//TODO: scatter with prob p and attenuation = albedo/p ??
	glm::vec3 normal = getNormal(hitRecord);
	glm::vec3 targetPoint = hitRecord.point + normal + glm::ballRand(1.0f);
	glm::vec3 scatter = targetPoint - hitRecord.point;
	scattered.set(hitRecord.point, scatter);
	
	float cos = glm::dot(normal, glm::normalize(scatter));
	attenuation = albedo->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV) * cos;
		
	return true;
}
