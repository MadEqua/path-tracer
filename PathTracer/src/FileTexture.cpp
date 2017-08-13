#include "FileTexture.h"

#include <iostream>
#include "Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

FileTexture::FileTexture(const std::string &path) {
	data = stbi_load(path.c_str(), &width, &height, &numberOfComponents, 0);
	if (!data) {
		std::cout << "Error loading image " + path << ". Reason: " << stbi_failure_reason() << std::endl;
	}
}

FileTexture::~FileTexture() {
	if (data) {
		stbi_image_free(data);
	}
}

Vec3 FileTexture::value(float u, float v, float scaleU, float scaleV) const {
	int s = static_cast<int>(Utils::decimalPart(u * scaleU) * static_cast<float>(width - 1));
	int t = static_cast<int>(Utils::decimalPart(v * scaleV) * static_cast<float>(height - 1));

	uint32 invertedY = height - 1 - t;
	byte *texel = data + ((invertedY * width) + s) * numberOfComponents;
	return Vec3(static_cast<float>(*texel) / 255.999f,
		static_cast<float>(*(texel + 1)) / 255.999f,
		static_cast<float>(*(texel + 2)) / 255.999f);
}
