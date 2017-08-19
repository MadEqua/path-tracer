#include "AABB.h"

#include <algorithm>

#include "Ray.h"
#include "Utils.h"


AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {
}

void AABB::enclose(const AABB &aabb) {
	min.x = Utils::min(min.x, aabb.min.x);
	min.y = Utils::min(min.y, aabb.min.y);
	min.z = Utils::min(min.z, aabb.min.z);

	max.x = Utils::max(max.x, aabb.max.x);
	max.y = Utils::max(max.y, aabb.max.y);
	max.z = Utils::max(max.z, aabb.max.z);
}

void AABB::enclose(const glm::vec3 &point) {
	min.x = Utils::min(min.x, point.x);
	min.y = Utils::min(min.y, point.y);
	min.z = Utils::min(min.z, point.z);

	max.x = Utils::max(max.x, point.x);
	max.y = Utils::max(max.y, point.y);
	max.z = Utils::max(max.z, point.z);
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
