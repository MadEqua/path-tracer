#include "Lambertian.h"

#include "Utils.h"
#include "Vec3.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Texture.h"

Lambertian::Lambertian(const Texture *texture, float textureScaleU, float textureScaleV) : 
	Material(texture, textureScaleU, textureScaleV) {
}

bool Lambertian::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	//TODO: scatter with prob p and attenuation = albedo/p ??
	Vec3 targetPoint = hitRecord.point + hitRecord.normal + Utils::randomInUnitSphere();
	scattered.set(hitRecord.point, targetPoint - hitRecord.point);
	attenuation = albedo->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV);
	return true;
}
