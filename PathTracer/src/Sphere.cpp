#include "Sphere.h"

#include <cmath>
#include <glm/glm.hpp>

#include "HitRecord.h"
#include "Ray.h"
#include "AABB.h"
#include "RenderStatistics.h"
#include "Utils.h"


Sphere::Sphere(const glm::vec3 &center, float radius, Material *material) :
	Object(material),
	center(center), radius(radius) {
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	statistics.objectIntersectionsComputed++;
	
	glm::vec3 oc = ray.origin - center;
	float a = glm::dot(ray.direction, ray.direction);
	float b = glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;

	float t;
	if (discriminant > 0.0f) {
		t = (-b - sqrt(discriminant)) / a;
		if (t > tMin && t < tMax) {
			computeHitRecord(ray, t, hitRecord);
			return true;
		}
		else {
			t = (-b + sqrt(discriminant)) / a;
			if (t > tMin && t < tMax) {
				computeHitRecord(ray, t, hitRecord);
				return true;
			}
		}
	}
	statistics.objectIntersectionsFailed++;
	return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB &aabb) const {
	glm::vec3 radiusVec(radius);
	aabb.set(center - radiusVec, center + radiusVec);
	return true;
}

void Sphere::computeHitRecord(const Ray &ray, float t, HitRecord &hitRecord) const {
	
	hitRecord.t = t;
	hitRecord.point = ray.pointAtParameter(t);
	
	glm::vec3 localCoord = hitRecord.point - center;
	float theta = acos(localCoord.y / radius); //[0,pi]
	float phi = atan2(localCoord.z, localCoord.x); //[-pi,+pi]

	hitRecord.u = 1.0f - ((phi + PI) / (2.0f * PI));
	hitRecord.v = 1.0f - (theta / PI);

	const glm::vec3 phiAxis(0, 1, 0);
	hitRecord.tangent = glm::normalize(glm::cross(phiAxis, localCoord));
	hitRecord.normal = localCoord / radius;
	hitRecord.bitangent = glm::normalize(glm::cross(hitRecord.normal, hitRecord.tangent));

	hitRecord.material = material;
}
