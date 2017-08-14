#include "Material.h"

#include "HitRecord.h"
#include "Mat3.h"
#include "Utils.h"
#include "Texture.h"

Vec3 Material::getNormal(const HitRecord &hitRecord) const {
	if (normalMap == nullptr)
		return hitRecord.normal;
	else {
		Mat3 tbn(hitRecord.tangent, hitRecord.bitangent, hitRecord.normal);
		Vec3 normal = Utils::unpackNormalFromRgb(normalMap->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV));
		return tbn * normal;
	}
}
