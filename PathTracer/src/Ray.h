#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Ray 
{
public:
	Ray() = default;
	Ray(const glm::vec3 &origin, const glm::vec3 &direction);
	Ray(const Ray& other, const glm::mat4 &transform); //Create a Ray transformed by a matrix

	glm::vec3 origin;
	glm::vec3 direction;

	void set(const glm::vec3 &origin, const glm::vec3 &direction);
	inline glm::vec3 pointAtParameter(float t) const { return origin + t * direction; }
};