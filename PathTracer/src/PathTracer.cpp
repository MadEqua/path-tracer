#include "PathTracer.h"

#include <iostream>

#include "Ray.h"
#include "Vec3.h"
#include "Scene.h"
#include "Camera.h"
#include "Utils.h"
#include "HitRecord.h"
#include "Material.h"
#include "Ray.h"
#include "Types.h"
#include "Timer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


PathTracer::PathTracer(const RenderSettings &settings, const Scene &scene) :
	settings(settings), scene(scene) {
}

PathTracer::~PathTracer() {
}

void PathTracer::renderScene() const {
	byte *image = new byte[settings.width * settings.height * 3];
	byte *ptr = image;

	Ray ray;
	Vec3 color;

	std::cout << "Rendering " << scene.getObjectCount() << " objects to " << settings.outputFileName << std::endl <<
		"Dimensions: " << settings.width << "x" << settings.height << std::endl <<
		"Samples: " << settings.samples << std::endl <<
		"Max ray depth: " << settings.maxRayDepth << std::endl << std::endl;

	const uint32 totalPixels = settings.width * settings.height;
	const uint32 updateIntervalMs = 2000;
	uint32 currentPixel = 0;
	uint32 currentPixelOnLastUpdate = 0;
	
	Timer timer;
	timer.start();

	for (int y = settings.height - 1; y >= 0; --y) {
		for (uint32 x = 0; x < settings.width; ++x) {
			
			if (timer.getElapsedMilis() > updateIntervalMs) {
				uint32 pixelsRendered = currentPixel - currentPixelOnLastUpdate;
				currentPixelOnLastUpdate = currentPixel;

				uint32 timePerPixelMs = updateIntervalMs / pixelsRendered;
				uint32 estimatedTimeMs = (totalPixels - currentPixel) * timePerPixelMs;
				
				std::cout << "Render progress: " <<
					static_cast<float>(currentPixel) / static_cast<float>(totalPixels) * 100.0f
					<< "% (estimated time to finish: " <<
					static_cast<float>(estimatedTimeMs) / 1000.0f / 60.0f << " minutes)" <<
					std::endl;

				timer.start();
			}
					
			color.set(0.0f);

			for (uint32 s = 0; s < settings.samples; ++s) {
				float u = static_cast<float>(x + Utils::random0To1()) / static_cast<float>(settings.width - 1.0f);
				float v = static_cast<float>(y + Utils::random0To1()) / static_cast<float>(settings.height - 1.0f);

				ray = scene.getCamera()->getRay(u, v);
				color += computeColor(ray, 0);
			}

			color /= static_cast<float>(settings.samples);
			Utils::rgbToSrgb(color);
			*ptr++ = static_cast<byte>(255.99f * color.r);
			*ptr++ = static_cast<byte>(255.99f * color.g);
			*ptr++ = static_cast<byte>(255.99f * color.b);

			currentPixel++;
		}
	}

	stbi_write_bmp(settings.outputFileName.c_str(), settings.width, settings.height, 3, image);
	delete[] image;
}

Vec3 PathTracer::computeColor(Ray &ray, uint32 depth) const {

	HitRecord hitRecord;

	if (scene.hit(ray, FLOAT_BIAS, FLT_MAX, hitRecord)) {

		Ray scattered;
		Vec3 attenuation;

		if (depth < settings.maxRayDepth && hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
			return attenuation * computeColor(scattered, depth + 1);
		}
		else {
			return Vec3(0.0f);
		}
	}

	ray.direction.normalize();
	float t = 0.5f * (ray.direction.y + 1.0f);
	return (1.0f - t) * Vec3(1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}