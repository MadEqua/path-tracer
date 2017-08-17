#include "Material.h"

#include "HitRecord.h"
#include "Mat3.h"
#include "Utils.h"
#include "Texture.h"

Material::Material() : albedo(nullptr), normalMap(nullptr) {
}

Material::Material(const Texture *albedo, float textureScaleU, float textureScaleV, bool emissive) :
	albedo(albedo), normalMap(nullptr),
	textureScaleU(textureScaleU), textureScaleV(textureScaleV),
	emissive(emissive) {
}

Material::Material(const Texture *albedo, const Texture *normalMap, float textureScaleU, float textureScaleV, bool emissive) :
	albedo(albedo), normalMap(normalMap),
	textureScaleU(textureScaleU), textureScaleV(textureScaleV),
	emissive(emissive) {
}

Vec3 Material::emit(float u, float v) const {
	if (emissive)
		return albedo->value(u, v, textureScaleU, textureScaleV);
	else
		return Vec3(0.0f);
}

Vec3 Material::getNormal(const HitRecord &hitRecord) const {
	if (normalMap == nullptr) {
		return hitRecord.normal;
	}
	else {
		Mat3 tbn(hitRecord.tangent, hitRecord.bitangent, hitRecord.normal);
		Vec3 normal = Utils::unpackNormalFromRgb(normalMap->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV));
		return tbn * normal;
	}
}
