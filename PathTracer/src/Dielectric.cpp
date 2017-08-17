#include "Dielectric.h"

#include "Utils.h"
#include "HitRecord.h"
#include "Vec3.h"
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

bool Dielectric::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	
	Vec3 normal = getNormal(hitRecord);
	
	Vec3 outwardNormal;
	float niOverNt;
	float cosine;

	Vec3 inDirection = in.direction;
	inDirection.normalize();

	if (inDirection.dot(normal) > 0.0f) { //outside
		outwardNormal = -normal;
		niOverNt = refractionIndex;
		cosine = refractionIndex * inDirection.dot(normal);
	}
	else { //inside
		outwardNormal = normal;
		niOverNt = 1.0f / refractionIndex;
		cosine = -inDirection.dot(normal);
	}

	Vec3 reflected = Utils::reflect(inDirection, normal);
	Vec3 refracted;

	if (Utils::refract(inDirection, outwardNormal, niOverNt, refracted)) {
		float reflectProb = Utils::shlick(cosine, refractionIndex);

		if (Utils::random0To1() < reflectProb) {
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
		attenuation.set(1.0f);
	}

	return true;
}
