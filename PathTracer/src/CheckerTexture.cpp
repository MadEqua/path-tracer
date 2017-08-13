#include "CheckerTexture.h"
#include "Utils.h"

#include <cmath>

CheckerTexture::CheckerTexture(const Vec3 &color1, const Vec3 &color2) :
	color1(color1), color2(color2) {
}

Vec3 CheckerTexture::value(float u, float v, float scaleU, float scaleV) const {
	return ((Utils::decimalPart(u * scaleU) < 0.5f) ^ 
		(Utils::decimalPart(v * scaleV) < 0.5f)) ? color1 : color2;
}
