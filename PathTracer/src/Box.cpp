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

	rectangles[0] = new Rectangle(Axis::X, 1.0f, 180.0f, 2.0f, 2.0f, material);
	rectangles[1] = new Rectangle(Axis::X, -1.0f, 0.0f, 2.0f, 2.0f, material);

	rectangles[2] = new Rectangle(Axis::Y, 1.0f, 0.0f, 2.0f, 2.0f, material);
	rectangles[3] = new Rectangle(Axis::Y, -1.0f, 180.0f, 2.0f, 2.0f, material);

	rectangles[4] = new Rectangle(Axis::Z, 1.0f, 180.0f, 2.0f, 2.0f, material);
	rectangles[5] = new Rectangle(Axis::Z, -1.0f, 0.0f, 2.0f, 2.0f, material);

	for (int i = 0; i < 6; ++i)
		rectangles[i]->computeTransformMatrices();
}

Box::~Box() {
	for (int i = 0; i < 6; ++i) 
		delete rectangles[i];
}

bool Box::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	statistics.objectIntersectionsComputed++;

	Ray localRay(ray, transform.getWorldToLocalMatrix());

	bool hit = false;
	HitRecord tempRecord;
	for (int i = 0; i < 6; ++i) {
		if (rectangles[i]->hit(localRay, tMin, tMax, tempRecord, statistics)) {
			hit = true;
			hitRecord = tempRecord;
			tMax = tempRecord.t;
		}
	}

	if (!hit) {
		statistics.objectIntersectionsFailed++;
		return false;
	}

	hitRecord.point = ray.pointAtParameter(hitRecord.t);
	//hitRecord.t = t;
	//hitRecord.material = material;

	//hitRecord.u = (a + 1.0f) / 2.0f;
	//hitRecord.v = (b + 1.0f) / 2.0f;

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
