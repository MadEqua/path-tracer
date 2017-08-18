#include "CheckerTexture.h"

#include "Utils.h"

CheckerTexture::CheckerTexture(const glm::vec3 &color1, const glm::vec3 &color2) :
	color1(color1), color2(color2) {
}

glm::vec3 CheckerTexture::value(float u, float v, float scaleU, float scaleV) const {
	return ((Utils::decimalPart(u * scaleU) < 0.5f) ^ 
		(Utils::decimalPart(v * scaleV) < 0.5f)) ? color1 : color2;
}
