#include "ConstantTexture.h"


ConstantTexture::ConstantTexture(const Vec3 &color) : color(color) {
}

Vec3 ConstantTexture::value(float u, float v, float scaleU, float scaleV) const {
	return color;
}
