#include "Rectangle.h"

#include"HitRecord.h"
#include "Ray.h"
#include "Types.h"
#include "AABB.h"

Rectangle::Rectangle(Axis axis, float distToOrigin, Material *material) :
	Object(material), axis(axis),
	distToOrigin(distToOrigin),
	minA(-INF_FLOAT32), maxA(INF_FLOAT32),
	minB(-INF_FLOAT32), maxB(INF_FLOAT32) {
}

Rectangle::Rectangle(Axis axis, float distToOrigin, float minA, float maxA, float minB, float maxB, Material *material) :
	Object(material), axis(axis), 
	distToOrigin(distToOrigin),
	minA(minA), maxA(maxA),
	minB(minB), maxB(maxB) {
}

bool Rectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {
	float rayOrigin = ray.origin[axis];
	float rayDirection = ray.direction[axis];

	float t = (distToOrigin - rayOrigin) / rayDirection;
	if (t < 0.0f || t < tMin || t > tMax) {
		return false;
	}
	else {
		hitRecord.point = ray.pointAtParameter(t);

		float a = hitRecord.point[(axis + 1) % 3];
		float b = hitRecord.point[(axis + 2) % 3];

		//The point is on the plane but not on the rectangle
		if (a < minA || a > maxA || b < minB || b > maxB) {
			return false;
		}

		hitRecord.t = t;
		hitRecord.material = material;

		if (maxA == INF_FLOAT32 || minA == -INF_FLOAT32) {
			hitRecord.v = a;
		}
		else {
			hitRecord.v = (a - maxA) / (maxA - minA);
		}

		if (maxB == INF_FLOAT32 || minB == -INF_FLOAT32) {
			hitRecord.u = b;
		}
		else {
			hitRecord.u = (b - maxB) / (maxB - minB);
		}

		switch (axis)
		{
		case X:
			hitRecord.tangent = glm::vec3(0.0f, 0.0f, 1.0f);
			hitRecord.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			hitRecord.bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case Y:
			hitRecord.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			hitRecord.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			hitRecord.bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case Z:
			hitRecord.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			hitRecord.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			hitRecord.bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}
		return true;
	}
}

bool Rectangle::boundingBox(float t0, float t1, AABB &aabb) const {
	switch (axis)
	{
	case X:
		aabb.min = glm::vec3(distToOrigin - 0.01f, minA, minB);
		aabb.max = glm::vec3(distToOrigin + 0.01f, maxA, maxB);
		break;
	case Y:
		aabb.min = glm::vec3(minA, distToOrigin - 0.01f, minB);
		aabb.max = glm::vec3(maxA, distToOrigin + 0.01f, maxB);
		break;
	case Z:
		aabb.min = glm::vec3(minA, minB, distToOrigin - 0.01f);
		aabb.max = glm::vec3(maxA, maxB, distToOrigin + 0.01f);
		break;
	}

	return true;
}
