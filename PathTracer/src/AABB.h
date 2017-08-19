#pragma once

#include <glm/vec3.hpp>

class Ray;

class AABB
{
public:
	AABB() = default;
	AABB(const glm::vec3 &min, const glm::vec3 &max);

	inline void set(const glm::vec3 &min, const glm::vec3 &max) { this->min = min; this->max = max; }
	void enclose(const AABB &aabb);
	void enclose(const glm::vec3 &point);

	bool hit(const Ray &ray, float tMin, float tMax) const;

	glm::vec3 min, max;
};