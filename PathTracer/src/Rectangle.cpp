#include "Rectangle.h"

#include"HitRecord.h"
#include "Ray.h"
#include "Types.h"
#include "AABB.h"
#include "RenderStatistics.h"


Rectangle::Rectangle(Axis axis, float distToOrigin, float rotation, float width, float height, Material *material) :
	Object(material), axis(axis) {

	switch (axis) {
	case X:
		transform.setPosition(glm::vec3(distToOrigin, 0.0f, 0.0f));
		transform.yaw(-90.0f + rotation);
		break;
	case Y:
		transform.setPosition(glm::vec3(0.0f, distToOrigin, 0.0f));
		transform.pitch(-90.0f + rotation);
		break;
	case Z:
		transform.setPosition(glm::vec3(0.0f, 0.0f, distToOrigin));
		transform.roll(rotation);
		break;
	}
	transform.setScale(glm::vec3(width / 2.0f, height / 2.0f, 1.0f));
}

bool Rectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	statistics.objectIntersectionsComputed++;

	Ray localRay(ray, transform.getWorldToLocalMatrix());

	if (localRay.direction.z == 0.0f) {
		statistics.objectIntersectionsFailed++;
		return false;
	}

	float t = -localRay.origin.z / localRay.direction.z;
	if (t < tMin || t > tMax) {
		statistics.objectIntersectionsFailed++;
		return false;
	}

	glm::vec3 localPoint = localRay.pointAtParameter(t);

	float a = localPoint.x;
	float b = localPoint.y;

	//The point is on the plane but not on the rectangle
	if (a < -1.0f || a > 1.0f || b < -1.0f || b > 1.0f) {
		statistics.objectIntersectionsFailed++;
		return false;
	}

	hitRecord.point = ray.pointAtParameter(t);
	hitRecord.t = t;
	hitRecord.material = material;

	hitRecord.u = (a + 1.0f) / 2.0f;
	hitRecord.v = (b + 1.0f) / 2.0f;

	hitRecord.tangent = glm::normalize(transform.getNormalMatrix() * glm::vec3(1.0f, 0.0f, 0.0f));
	hitRecord.normal = glm::normalize(transform.getNormalMatrix() * glm::vec3(0.0f, 0.0f, 1.0f));
	hitRecord.bitangent = glm::normalize(glm::cross(hitRecord.normal, hitRecord.tangent));

	return true;
}

void Rectangle::computeAABB() const {
	
	glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
	aabb.min = aabb.max = transform.getPosition();
	
	for (int x = 0; x < 2; ++x) {
		for (int y = 0; y < 2; ++y) {
			for (int z = 0; z < 2; ++z) {
				point.x = (x == 0 ? 1.0f : -1.0f);
				point.y = (y == 0 ? 1.0f : -1.0f);
				point.z = (z == 0 ? 0.01f : -0.01f);

				point = transform.getLocalToWorldMatrix() * point;
				aabb.enclose(point);
			}
		}
	}
}
