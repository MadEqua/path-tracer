#include "ConstantTexture.h"


ConstantTexture::ConstantTexture(const glm::vec3 &color) : color(color) {
}

glm::vec3 ConstantTexture::value(float u, float v, float scaleU, float scaleV) const {
	return color;
}
