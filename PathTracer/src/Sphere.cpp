#include "Sphere.h"

#include <cmath>

#include "HitRecord.h"
#include "Ray.h"
#include "AABB.h"
#include "RenderStatistics.h"
#include "Utils.h"


Sphere::Sphere(const Vec3 &center, float radius, Material *material) :
	Object(material),
	center(center), radius(radius) {
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
	Vec3 radiusVec(radius);
	aabb.set(center - radiusVec, center + radiusVec);
	return true;
}

void Sphere::computeHitRecord(const Ray &ray, float t, HitRecord &hitRecord) const {
	
	hitRecord.t = t;
	hitRecord.point = ray.pointAtParameter(t);
	
	Vec3 localCoord = hitRecord.point - center;
	float theta = asin(localCoord.y / radius); //[-pi/2,pi/2]
	float phi = atan2(localCoord.z, localCoord.x); //[-pi,+pi]

	//TODO find a better method to compute tangents
	float deltaPhi = phi + FLOAT_BIAS;
	Vec3 deltaPoint(radius * cos(theta) * cos(deltaPhi),
		radius * sin(theta),
		radius * cos(theta) * sin(deltaPhi));

	//Assume local space to be aligned with world space
	//So this is also the tangent in world space
	hitRecord.tangent = deltaPoint - localCoord;
	hitRecord.tangent.normalize();

	hitRecord.normal = localCoord / radius;
	hitRecord.bitangent = hitRecord.tangent.cross(hitRecord.normal);
	hitRecord.bitangent.normalize();

	hitRecord.material = material;

	hitRecord.u = 1.0f - ((phi + PI) / (2.0f * PI));
	hitRecord.v = (theta + PI / 2.0f) / PI;
}
