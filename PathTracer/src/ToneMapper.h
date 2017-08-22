#pragma once

#include <glm/glm.hpp>

class ToneMapper
{
public:
	ToneMapper(const float *image, int width, int height, float key, float lumWhite);

	glm::u8vec3 mapColor(const glm::vec3 &color);

private:
	const float *image;
	int width, height;
	float averageLuminosity;
	float key;
	float lumWhiteSquared;
};

