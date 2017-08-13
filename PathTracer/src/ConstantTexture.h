#pragma once

#include "Texture.h"

class ConstantTexture : public Texture
{
public:
	explicit ConstantTexture(const Vec3 &color);
	virtual Vec3 value(float u, float v, float scaleU, float scaleV) const override;

private:
	Vec3 color;
};

