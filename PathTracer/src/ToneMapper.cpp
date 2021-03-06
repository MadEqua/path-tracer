#include "ToneMapper.h"

#include <cmath>
#include "Utils.h"

ToneMapper::ToneMapper(const float *image, int width, int height, float key, float lumWhite) :
	image(image), width(width), height(height),
	key(key),
	averageLuminosity(0.0f),
	lumWhiteSquared(lumWhite * lumWhite) {
	
	const float SMALL_FLOAT = 0.00000001f;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			glm::vec3 color = Utils::getPixelFromImage(image, width, height, x, y);
			averageLuminosity += log(SMALL_FLOAT + Utils::getLuminanceFromRgb(color));
		}
	}
	averageLuminosity = exp(averageLuminosity / static_cast<float>(width * height));
}

glm::u8vec3 ToneMapper::mapColor(const glm::vec3 &color) {
	float lum = Utils::getLuminanceFromRgb(color);
	float newLum = (key * lum) / averageLuminosity;

	newLum = (newLum * (1.0f + (newLum / lumWhiteSquared))) / (1.0f + newLum);

	//the colorNew/colorOld has the same ratio as lumNew/lumOld
	float ratio = newLum / lum;
	glm::vec3 newColor = color * ratio;

	newColor = glm::clamp(newColor, 0.0f, 1.0f);
	glm::vec3 srgb = Utils::rgbToSrgb(newColor);
	glm::u8vec3 byteColor = srgb * 255.999f;
	return byteColor;
}
