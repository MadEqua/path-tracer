#include "Sphere.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "HitRecord.h"
#include "Ray.h"
#include "AABB.h"
#include "RenderStatistics.h"
#include "Utils.h"

Sphere::Sphere(const glm::vec3 &center, float radius, Material *material) :
	Object(material) {
	transform.setPosition(center);
	transform.setScale(glm::vec3(radius));
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	
	Ray localRay(ray, transform.getWorldToLocalMatrix());
	
	statistics.objectIntersectionsComputed++;
	
	float a = glm::dot(localRay.direction, localRay.direction);
	float b = glm::dot(localRay.origin, localRay.direction);
	float c = glm::dot(localRay.origin, localRay.origin) - 1.0f;
	float discriminant = b * b - a * c;

	float t;
	if (discriminant > 0.0f) {
		t = (-b - sqrt(discriminant)) / a;
		if (t > tMin && t < tMax) {
			computeHitRecord(ray, localRay, t, hitRecord);
			return true;
		}
		else {
			t = (-b + sqrt(discriminant)) / a;
			if (t > tMin && t < tMax) {
				computeHitRecord(ray, localRay, t, hitRecord);
				return true;
			}
		}
	}
	statistics.objectIntersectionsFailed++;
	return false;
}

void Sphere::computeAABB() const {

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

void Sphere::computeHitRecord(const Ray &worldRay, const Ray &localRay, float t, HitRecord &hitRecord) const {
	
	hitRecord.t = t;
	hitRecord.point = worldRay.pointAtParameter(t);

	glm::vec3 localPoint = localRay.pointAtParameter(t);
	float theta = acos(localPoint.y); //[0,pi]
	float phi = atan2(localPoint.z, localPoint.x); //[-pi,+pi]

	hitRecord.u = 1.0f - ((phi + PI) / (2.0f * PI));
	hitRecord.v = 1.0f - (theta / PI);

	const glm::vec3 phiAxis(0, 1, 0);
	hitRecord.tangent = glm::normalize(transform.getNormalMatrix() * glm::cross(phiAxis, localPoint));
	hitRecord.normal = glm::normalize(transform.getNormalMatrix() * localPoint);
	hitRecord.bitangent = glm::normalize(glm::cross(hitRecord.normal, hitRecord.tangent));

	hitRecord.material = material;
}
