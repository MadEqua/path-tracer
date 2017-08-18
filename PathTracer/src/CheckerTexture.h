#pragma once

#include "Texture.h"

class CheckerTexture :public Texture
{
public:
	CheckerTexture(const glm::vec3 &color1, const glm::vec3 &color2);
	virtual glm::vec3 value(float u, float v, float scaleU, float scaleV) const override;

private:
	glm::vec3 color1;
	glm::vec3 color2;
};
