#pragma once

#include "Texture.h"
#include "Types.h"
#include <string>

class FileTexture : public Texture
{
public:
	FileTexture(const std::string &path, bool sRGB = true);
	virtual ~FileTexture();
	virtual glm::vec3 value(float u, float v, float scaleU, float scaleV) const override;

private:
	byte *data;
	int width, height;
	int numberOfComponents; //ex: RGB = 3
	bool sRGB;
};

