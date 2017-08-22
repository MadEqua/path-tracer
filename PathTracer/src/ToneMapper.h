#pragma once

#include <glm/glm.hpp>

class ToneMapper
{
public:
	ToneMapper(const float *image, int width, int height, float exposure);

	glm::u8vec3 mapColor(const glm::vec3 &color);

private:
	const float *image;
	int width, height;
	float averageLuminosity;
	float exposure;
};

