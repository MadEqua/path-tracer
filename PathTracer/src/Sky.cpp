#include "Sky.h"

#include "Ray.h"
#include "Utils.h"

#include <cmath>

#include "stb_image.h"

Sky::Sky(const std::string &file) {
	skyData = stbi_loadf(file.c_str(), &w, &h, &n, 0);
}

Sky::~Sky() {
	delete[] skyData;
}

glm::vec3 Sky::getColor(const glm::vec3 &direction) const {
	float d = sqrt(direction.x * direction.x + direction.y * direction.y);
	float r = d > 0 ? (1.0f / PI) * acos(direction.z) / d : 0.0f;
	float u = (direction.x * r + 1.0f) / 2.0f;
	float v = (direction.y * r + 1.0f) / 2.0f;
	int x = (int)(u * (float)w);
	int y = (int)(v * (float)h);
	return Utils::getPixelFromImage(skyData, w, h, x, y);
}
