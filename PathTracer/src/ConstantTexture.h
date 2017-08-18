#pragma once

#include "Texture.h"

class ConstantTexture : public Texture
{
public:
	explicit ConstantTexture(const glm::vec3 &color);
	virtual glm::vec3 value(float u, float v, float scaleU, float scaleV) const override;

private:
	glm::vec3 color;
};

