#include "Dielectric.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Utils.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Texture.h"

Dielectric::Dielectric(float refractionIndex) : 
	Material(false),
	refractionIndex(refractionIndex) {
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
	
	glm::vec3 inDirection = glm::normalize(in.direction);
	float inDotN = glm::dot(inDirection, normal);

	glm::vec3 outNormal;
	float niOverNt;
	float cosine;

	if (inDotN > 0.0f) { //coming from inside
		outNormal = -normal;
		niOverNt = refractionIndex;
		cosine = inDotN;
	}
	else { //coming from outside
		outNormal = normal;
		niOverNt = 1.0f / refractionIndex;
		cosine = -inDotN;
	}

	glm::vec3 reflected = glm::reflect(inDirection, outNormal);
	glm::vec3 refracted;

	if (Utils::refract(inDirection, outNormal, niOverNt, refracted)) {
		float reflectProb = Utils::shlick(cosine, refractionIndex);

		if (glm::linearRand(0.0f, 1.0f) < reflectProb) {
			scattered.set(hitRecord.point, reflected);
		}
		else {
			scattered.set(hitRecord.point, refracted);
		}
	}
	else { //Total internal reflection
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
