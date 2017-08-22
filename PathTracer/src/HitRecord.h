#pragma once

#include <glm/vec3.hpp>

class Material;

struct HitRecord {
	float t;
	glm::vec3 point;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 normal;
	float u, v; //Texture coordinates
	const Material *material;
};