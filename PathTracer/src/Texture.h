#pragma once

#include <glm/vec3.hpp>

class Texture
{
public:
	virtual ~Texture() = default;
	virtual glm::vec3 value(float u, float v, float scaleU, float scaleV) const = 0;
};

