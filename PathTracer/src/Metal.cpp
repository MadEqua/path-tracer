#include "Metal.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "HitRecord.h"
#include "Ray.h"
#include "Utils.h"
#include "Texture.h"


Metal::Metal(const Texture *albedo, float fuzziness, float textureScaleU, float textureScaleV) : 
	Metal(albedo, nullptr, fuzziness, textureScaleU, textureScaleV) {
}

Metal::Metal(const Texture *albedo, const Texture *normalMap, float fuzziness, float textureScaleU, float textureScaleV) :
	Material(albedo, normalMap, textureScaleU, textureScaleV, false) {
	if (fuzziness < 1.0f)
		this->fuzziness = fuzziness;
	else
		this->fuzziness = 1.0f;
}

bool Metal::scatter(const Ray &in, const HitRecord &hitRecord, glm::vec3 &attenuation, Ray &scattered) const {
	
	glm::vec3 normal = getNormal(hitRecord);

	glm::vec3 inDirection = glm::normalize(in.direction);
	scattered.set(hitRecord.point, glm::normalize(glm::reflect(inDirection, normal) + fuzziness * glm::ballRand(1.0f)));

	if (glm::dot(scattered.direction, hitRecord.normal) > 0.0f) {
		attenuation = albedo->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV);
		return true;
	}
	//Absorb possible reflections to the "inside" 
	//Possible due to the fuzziness random operation and normal maps
	//Actually, normal mapping creates a lot of rays that should be self-reflected but will be "absorbed"...
	else {
		return false;
	}
}