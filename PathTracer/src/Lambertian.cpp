#include "Lambertian.h"

#include <random>

#include "Utils.h"
#include "Vec3.h"
#include "HitRecord.h"
#include "Ray.h"

Lambertian::Lambertian(const Vec3 &albedo) : albedo(albedo) {
}

Lambertian::~Lambertian() {
}

bool Lambertian::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	//TODO: scatter with prob p and attenuation = albedo/p ??
	Vec3 targetPoint = hitRecord.point + hitRecord.normal + Utils::randomInUnitSphere();
	scattered.set(hitRecord.point, targetPoint - hitRecord.point);
	attenuation = albedo;
	return true;
}
