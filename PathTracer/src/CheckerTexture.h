#pragma once

#include "Texture.h"

class CheckerTexture :public Texture
{
public:
	CheckerTexture(const Vec3 &color1, const Vec3 &color2);
	virtual Vec3 value(float u, float v, float scaleU, float scaleV) const override;

private:
	Vec3 color1;
	Vec3 color2;
};
