#include "Metal.h"

#include "Vec3.h"
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

bool Metal::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	
	Vec3 normal = getNormal(hitRecord);
	
	Vec3 inDirection = in.direction;
	inDirection.normalize();
	
	scattered.set(hitRecord.point, Utils::reflect(inDirection, normal) + fuzziness * Utils::randomInUnitSphere());
	scattered.direction.normalize();

	if (scattered.direction.dot(hitRecord.normal) > 0.0f) {
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