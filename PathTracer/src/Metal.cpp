#include "Metal.h"

#include "Vec3.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Utils.h"

Metal::Metal(const Vec3 &albedo, float fuzziness) : albedo(albedo) {
	if (fuzziness < 1.0f)
		this->fuzziness = fuzziness;
	else 
		this->fuzziness = 1.0f;
}

Metal::~Metal() {
}

bool Metal::scatter(const Ray &in, const HitRecord &hitRecord, Vec3 &attenuation, Ray &scattered) const {
	
	Vec3 inDirCopy = in.direction;
	inDirCopy.normalize();

	scattered.set(hitRecord.point, Utils::reflect(inDirCopy, hitRecord.normal) + fuzziness * Utils::randomInUnitSphere());
	attenuation = albedo;

	//Absorb possible reflections to the "inside" (possible due to the fuzziness random operation)
	return scattered.direction.dot(hitRecord.normal) > 0.0f; 
}