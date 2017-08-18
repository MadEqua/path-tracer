#include "Dielectric.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Utils.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Texture.h"

Dielectric::Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {
}

Dielectric::Dielectric(const Texture *albedo, float refractionIndex, float textureScaleU, float textureScaleV) :
	Material(albedo, textureScaleU, textureScaleV, false), 
	refractionIndex(refractionIndex) {
}

Dielectric::Dielectric(const Texture *albedo, const Texture *normalMap, float refractionIndex, float textureScaleU, float textureScaleV) :
	Material(albedo, normalMap, textureScaleU, textureScaleV, false),
	refractionIndex(refractionIndex) {
}

bool Dielectric::scatter(const Ray &in, const HitRecord &hitRecord, glm::vec3 &attenuation, Ray &scattered) const {
	
	glm::vec3 normal = getNormal(hitRecord);
	
	glm::vec3 outwardNormal;
	float niOverNt;
	float cosine;

	glm::vec3 inDirection = glm::normalize(in.direction);

	if (glm::dot(inDirection, normal) > 0.0f) { //outside
		outwardNormal = -normal;
		niOverNt = refractionIndex;
		cosine = refractionIndex * glm::dot(inDirection, normal);
	}
	else { //inside
		outwardNormal = normal;
		niOverNt = 1.0f / refractionIndex;
		cosine = -glm::dot(inDirection, normal);
	}

	glm::vec3 reflected = glm::reflect(inDirection, normal);
	glm::vec3 refracted;

	if (Utils::refract(inDirection, outwardNormal, niOverNt, refracted)) {
		float reflectProb = Utils::shlick(cosine, refractionIndex);

		if (glm::linearRand(0.0f, 1.0f) < reflectProb) {
			scattered.set(hitRecord.point, reflected);
		}
		else {
			scattered.set(hitRecord.point, refracted);
		}

	}
	else {
		scattered.set(hitRecord.point, reflected);
	}

	if (albedo != nullptr) {
		attenuation = albedo->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV);
	}
	else {
		attenuation.x = attenuation.y = attenuation.z = 1.0f;
	}

	return true;
}
