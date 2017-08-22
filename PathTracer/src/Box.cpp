#include "Box.h"

#include "Ray.h"
#include "HitRecord.h"
#include "RenderStatistics.h"
#include "Transform.h"
#include "Utils.h"

Box::Box(const glm::vec3 &center, const glm::vec3 &dimensions, const glm::vec3 &rotation, Material *material) :
	Object(material) {

	transform.setPosition(center);
	transform.setScale(dimensions / 2.0f);
	transform.yaw(rotation.y);
	transform.pitch(rotation.x);
	transform.roll(rotation.z);
}

bool Box::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	statistics.objectIntersectionsComputed++;

	Ray localRay(ray, transform.getWorldToLocalMatrix());

	glm::vec3 min(-1.0f);
	glm::vec3 max(1.0f);
	
	for (int i = 0; i < 3; ++i) {
		float invD = 1.0f / localRay.direction[i];
		float t0 = (min[i] - localRay.origin[i]) * invD;
		float t1 = (max[i] - localRay.origin[i]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);

		tMin = Utils::max(t0, tMin);
		tMax = Utils::min(t1, tMax);

		if (tMax <= tMin) {
			statistics.objectIntersectionsFailed++;
			return false;
		}
	}

	hitRecord.t = tMin > 0.0f ? tMin : tMax;
	hitRecord.point = ray.pointAtParameter(hitRecord.t);
	hitRecord.material = material;

	glm::vec3 localPoint = localRay.pointAtParameter(hitRecord.t);

	if (abs(localPoint.x) > abs(localPoint.y) && abs(localPoint.x) > abs(localPoint.z)) {
		hitRecord.u = (hitRecord.point.z + 1.0f) / 2.0f;
		hitRecord.v = (hitRecord.point.y + 1.0f) / 2.0f;

		hitRecord.tangent = glm::vec3(0.0f, 0.0f, 1.0f);
		hitRecord.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		
		if (localPoint.x > 0.0f) {
			hitRecord.u = 1.0f - hitRecord.u;
			hitRecord.tangent.z = -hitRecord.tangent.z;
		}
		else {
			hitRecord.normal.x = -hitRecord.normal.x;
		}

	}
	else if (abs(localPoint.y) > abs(localPoint.x) && abs(localPoint.y) > abs(localPoint.z)) {
		hitRecord.u = (hitRecord.point.x + 1.0f) / 2.0f;
		hitRecord.v = (hitRecord.point.z + 1.0f) / 2.0f;

		hitRecord.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
		hitRecord.normal = glm::vec3(0.0f, 1.0f, 0.0f);

		if (localPoint.y < 0.0f) {
			hitRecord.u = 1.0f - hitRecord.u;
			hitRecord.normal.y = -hitRecord.normal.y;
		}
		else {
			hitRecord.v = 1.0f - hitRecord.v;
		}
	}
	else {
		hitRecord.u = (hitRecord.point.x + 1.0f) / 2.0f;
		hitRecord.v = (hitRecord.point.y + 1.0f) / 2.0f;

		hitRecord.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
		hitRecord.normal = glm::vec3(0.0f, 0.0f, 1.0f);

		if (localPoint.z < 0.0f) {
			hitRecord.u = 1.0f - hitRecord.u;
			hitRecord.tangent.x = -hitRecord.tangent.x;
			hitRecord.normal.z = -hitRecord.normal.z;
		}
	}

	hitRecord.tangent = glm::normalize(transform.getNormalMatrix() * hitRecord.tangent);
	hitRecord.normal = glm::normalize(transform.getNormalMatrix() * hitRecord.normal);
	hitRecord.bitangent = glm::normalize(glm::cross(hitRecord.normal, hitRecord.tangent));

	return true;
}

void Box::computeAABB() const {
	glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
	aabb.min = aabb.max = transform.getPosition();

	for (int x = 0; x < 2; ++x) {
		for (int y = 0; y < 2; ++y) {
			for (int z = 0; z < 2; ++z) {
				point.x = (x == 0 ? 1.0f : -1.0f);
				point.y = (y == 0 ? 1.0f : -1.0f);
				point.z = (z == 0 ? 1.0f : -1.0f);

				point = transform.getLocalToWorldMatrix() * point;
				aabb.enclose(point);
			}
		}
	}
}
