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


PathTracer::PathTracer(const RenderSettings &settings, Scene &scene) :
	settings(settings), scene(scene) {
}

PathTracer::~PathTracer() {
}

void PathTracer::renderScene() {
	scene.initializeBvh();

	byte *image = new byte[settings.width * settings.height * 3];

	Ray ray;
	Vec3 color;

	const int numXtiles = static_cast<int>(ceil(static_cast<float>(settings.width) / static_cast<float>(settings.tileSize)));
	const int numYtiles = static_cast<int>(ceil(static_cast<float>(settings.height) / static_cast<float>(settings.tileSize)));
	const int numTiles = numXtiles * numYtiles;

	const uint32 totalPixels = settings.width * settings.height;
	const float updateInterval = 5.0f;
	uint32 totalPixelsOnLastUpdate = 0;

	Timer updateTimer;
	updateTimer.start();
	statistics.timer.start();

	for (int tile = 0; tile < numTiles; tile++) {
		
		const int tileOffsetX = settings.tileSize * (tile % numXtiles);
		const int tileOffsetY = settings.tileSize * (tile / numXtiles);

		for (uint32 y = tileOffsetY; y < tileOffsetY + settings.tileSize && y < settings.height; y++) {
			for (uint32 x = tileOffsetX; x < tileOffsetX + settings.tileSize && x < settings.width; x++) {

				if (updateTimer.getElapsedSeconds() > updateInterval) {
					uint32 pixelsRendered = statistics.totalRenderedPixels - totalPixelsOnLastUpdate;
					totalPixelsOnLastUpdate = statistics.totalRenderedPixels;

					float timePerPixel = updateInterval / static_cast<float>(pixelsRendered);
					float estimatedTime = static_cast<float>((totalPixels - statistics.totalRenderedPixels)) * timePerPixel;

					std::cout << "Render progress: " <<
						static_cast<float>(statistics.totalRenderedPixels) / static_cast<float>(totalPixels) * 100.0f <<
						"% (estimated time to finish: " << static_cast<float>(estimatedTime) << " seconds)" <<
						std::endl;

					updateTimer.start();
				}

				color.set(0.0f);

				for (uint32 s = 0; s < settings.samples; ++s) {
					float u = static_cast<float>(x + Utils::random0To1()) / static_cast<float>(settings.width - 1.0f);
					float v = static_cast<float>(y + Utils::random0To1()) / static_cast<float>(settings.height - 1.0f);
					
					ray = scene.getCamera()->getRay(u, v);
					color += computeColor(ray, 0);
					statistics.primaryRays++;
				}

				color /= static_cast<float>(settings.samples);
				Utils::rgbToSrgb(color);
				
				uint32 invertedY = settings.height - 1 - y;
				byte *ptr = image + ((settings.width * invertedY) + x) * 3;
				*ptr++ = static_cast<byte>(255.99f * color.r);
				*ptr++ = static_cast<byte>(255.99f * color.g);
				*ptr = static_cast<byte>(255.99f * color.b);

				statistics.totalRenderedPixels++;
			}
		}
	}











	/*for (int y = settings.height - 1; y >= 0; --y) {
		for (uint32 x = 0; x < settings.width; ++x) {
			
			if (updateTimer.getElapsedSeconds() > updateInterval) {
				uint32 pixelsRendered = statistics.totalRenderedPixels - currentPixelOnLastUpdate;
				currentPixelOnLastUpdate = statistics.totalRenderedPixels;

				float timePerPixel = updateInterval / static_cast<float>(pixelsRendered);
				float estimatedTime = static_cast<float>((totalPixels - statistics.totalRenderedPixels)) * timePerPixel;
				
				std::cout << "Render progress: " <<
					static_cast<float>(statistics.totalRenderedPixels) / static_cast<float>(totalPixels) * 100.0f <<
					"% (estimated time to finish: " << static_cast<float>(estimatedTime) << " seconds)" <<
					std::endl;

				updateTimer.start();
			}
					
			color.set(0.0f);

			for (uint32 s = 0; s < settings.samples; ++s) {
				float u = static_cast<float>(x + Utils::random0To1()) / static_cast<float>(settings.width - 1.0f);
				float v = static_cast<float>(y + Utils::random0To1()) / static_cast<float>(settings.height - 1.0f);

				ray = scene.getCamera()->getRay(u, v);
				color += computeColor(ray, 0);
				statistics.primaryRays++;
			}

			color /= static_cast<float>(settings.samples);
			Utils::rgbToSrgb(color);
			*ptr++ = static_cast<byte>(255.99f * color.r);
			*ptr++ = static_cast<byte>(255.99f * color.g);
			*ptr++ = static_cast<byte>(255.99f * color.b);

			statistics.totalRenderedPixels++;
		}
	}*/

	stbi_write_bmp(settings.outputFileName.c_str(), settings.width, settings.height, 3, image);
	delete[] image;
}

Vec3 PathTracer::computeColor(Ray &ray, uint32 depth) {

	HitRecord hitRecord;
	statistics.maxDepthReached = Utils::max(statistics.maxDepthReached, depth);

	if (scene.hit(ray, FLOAT_BIAS, FLT_MAX, hitRecord, statistics)) {

		Ray scattered;
		Vec3 attenuation;

		if (depth < settings.maxRayDepth) {
			if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
				statistics.scatteredRays++;
				return attenuation * computeColor(scattered, depth + 1);
			}
			else {
				return Vec3(0.0f);
			}
		}
		else {
			statistics.raysReachedMaxDepth++;
			return Vec3(0.0f);
		}
	}

	ray.direction.normalize();
	float t = 0.5f * (ray.direction.y + 1.0f);
	return (1.0f - t) * Vec3(1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void PathTracer::printPreRender() const {
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << "Rendering " << scene.getObjectCount() << " objects" << std::endl;
	std::cout << settings << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
}

void PathTracer::printPostRender() const {
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << statistics << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
}