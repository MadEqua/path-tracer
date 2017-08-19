#include "Material.h"

#include "HitRecord.h"
#include "glm/mat3x3.hpp"
#include "Utils.h"
#include "Texture.h"

Material::Material(bool emissive) : 
	albedo(nullptr), normalMap(nullptr),
	emissive(emissive) {
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

glm::vec3 Material::emit(float u, float v) const {
	if (emissive)
		return albedo->value(u, v, textureScaleU, textureScaleV);
	else
		return glm::vec3(0.0f);
}

glm::vec3 Material::getNormal(const HitRecord &hitRecord) const {
	if (normalMap == nullptr) {
		return hitRecord.normal;
	}
	else {
		glm::mat3 tbn(hitRecord.tangent, hitRecord.bitangent, hitRecord.normal);
		glm::vec3 normal = Utils::unpackNormalFromRgb(normalMap->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV));
		return tbn * normal;
	}
}
