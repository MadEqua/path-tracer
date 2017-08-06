#include "AABB.h"

#include "Ray.h"
#include "Utils.h"


AABB::AABB(const Vec3 &min, const Vec3 &max) : min(min), max(max) {
}

void AABB::enclose(const AABB & aabb) {
	min.set(Utils::min(min.x, aabb.min.x),
		Utils::min(min.y, aabb.min.y),
		Utils::min(min.z, aabb.min.z));

	max.set(Utils::max(max.x, aabb.max.x),
		Utils::max(max.y, aabb.max.y),
		Utils::max(max.z, aabb.max.z));
}

bool AABB::hit(const Ray &ray, float tMin, float tMax) const {
	for (int i = 0; i < 3; ++i) {
		float invD = 1.0f / ray.direction[i];
		float t0 = (min[i] - ray.origin[i]) * invD;
		float t1 = (max[i] - ray.origin[i]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);

		tMin = Utils::max(t0, tMin);
		tMax = Utils::min(t1, tMax);

		if (tMax <= tMin) 
			return false;
	}
	return true;
}
