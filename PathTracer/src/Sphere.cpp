#include "Sphere.h"

#include "HitRecord.h"
#include "Ray.h"


Sphere::Sphere(const Vec3 &center, float radius, Material *material) :
	Object(material),
	center(center), radius(radius) {
}

Sphere::~Sphere() {
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const {
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
	return false;
}