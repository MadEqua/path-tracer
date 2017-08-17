#include "LightMaterial.h"

#include "Texture.h"

LightMaterial::LightMaterial(const Texture *emission, float textureScaleU, float textureScaleV) :
	Material(emission, textureScaleU, textureScaleV, true) {
}

bool LightMaterial::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	return false;
}
