#include "Dielectric.h"

#include "Utils.h"
#include "HitRecord.h"
#include "Vec3.h"
#include "Ray.h"

Dielectric::Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {
}

bool Dielectric::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	Vec3 outwardNormal;
	Vec3 reflected = Utils::reflect(in.direction, hitRecord.normal);
	float niOverNt;
	attenuation.set(1.0f);
	Vec3 refracted;
	float cosine;

	if (in.direction.dot(hitRecord.normal) > 0.0f) { //outside
		outwardNormal = -hitRecord.normal;
		niOverNt = refractionIndex;
		cosine = refractionIndex * in.direction.dot(hitRecord.normal) / in.direction.length();
	}
	else { //inside
		outwardNormal = hitRecord.normal;
		niOverNt = 1.0f / refractionIndex;
		cosine = -in.direction.dot(hitRecord.normal) / in.direction.length();
	}

	if (Utils::refract(in.direction, outwardNormal, niOverNt, refracted)) {
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

	return true;
}
