#include "Sphere.h"

#include "HitRecord.h"
#include "Ray.h"
#include "AABB.h"
#include "RenderStatistics.h"


Sphere::Sphere(const Vec3 &center, float radius, Material *material) :
	Object(material),
	center(center), radius(radius) {
}

Sphere::~Sphere() {
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	statistics.objectIntersectionsComputed++;
	
	Vec3 oc = ray.origin - center;
	float a = ray.direction.dot(ray.direction);
	float b = oc.dot(ray.direction);
	float c = oc.dot(oc) - radius * radius;
	float discriminant = b * b - a * c;

	float t;
	if (discriminant > 0.0f) {
		t = (-b - sqrt(discriminant)) / a;
		if (t > tMin && t < tMax) {
			hitRecord.t = t;
			hitRecord.point = ray.pointAtParameter(t);
			hitRecord.normal = (hitRecord.point - center) / radius;
			hitRecord.material = material;
			return true;
		}
		else {
			t = (-b + sqrt(discriminant)) / a;
			if (t > tMin && t < tMax) {
				hitRecord.t = t;
				hitRecord.point = ray.pointAtParameter(t);
				hitRecord.normal = (hitRecord.point - center) / radius;
				hitRecord.material = material;
				return true;
			}
		}
	}
	statistics.objectIntersectionsFailed++;
	return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB &aabb) const {
	Vec3 radiusVec(radius);
	aabb.set(center - radiusVec, center + radiusVec);
	return true;
}
