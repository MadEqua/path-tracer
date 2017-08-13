#include "Metal.h"

#include "Vec3.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Utils.h"
#include "Texture.h"

Metal::Metal(const Texture *albedo, float fuzziness, float textureScaleU, float textureScaleV) : 
	Material(albedo, textureScaleU, textureScaleV) {
	if (fuzziness < 1.0f)
		this->fuzziness = fuzziness;
	else 
		this->fuzziness = 1.0f;
}

bool Metal::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	
	Vec3 inDirCopy = in.direction;
	inDirCopy.normalize();

	scattered.set(hitRecord.point, Utils::reflect(inDirCopy, hitRecord.normal) + fuzziness * Utils::randomInUnitSphere());
	if (scattered.direction.dot(hitRecord.normal) > 0.0f) {
		attenuation = albedo->value(hitRecord.u, hitRecord.v, textureScaleU, textureScaleV);
		return true;
	}
	//Absorb possible reflections to the "inside" (possible due to the fuzziness random operation)
	else {
		return false;
	}
}