#pragma once

#include <glm/vec3.hpp>

class Ray 
{
public:
	Ray() = default;
	Ray(const glm::vec3 &origin, const glm::vec3 &direction);

	glm::vec3 origin;
	glm::vec3 direction;

	inline void set(const glm::vec3 &origin, const glm::vec3 &direction) { this->origin = origin; this->direction = direction; }
	inline glm::vec3 pointAtParameter(float t) const { return origin + t * direction; }
};