#pragma once

#include "Vec3.h"

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Vec3 value(float u, float v, float scaleU, float scaleV) const = 0;
};

