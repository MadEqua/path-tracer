#include "PathTracer.h"

#include <iostream>
#include <thread>

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
	settings(settings), 
	scene(scene),
	threadPool(settings.threads),
	threadStatistics(settings.threads) {
	
	scene.initializeBvh();
	imageBuffer = new byte[settings.width * settings.height * 3];
	currentTile = 0;
}

PathTracer::~PathTracer() {
	delete[] imageBuffer;
}

void PathTracer::renderScene() {

	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i] = std::thread(&PathTracer::renderTile, this, i);
	}

	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i].join();
	}

	stbi_write_bmp(settings.outputFileName.c_str(), settings.width, settings.height, 3, imageBuffer);
	//stbi_write_jpg((settings.outputFileName + ".jpg").c_str(), settings.width, settings.height, 3, imageBuffer, 100);
}

void PathTracer::renderTile(int threadId) {
	
	RenderStatistics &statistics = threadStatistics[threadId];
	
	const uint32 numXtiles = static_cast<int>(ceil(static_cast<float>(settings.width) / static_cast<float>(settings.tileSize)));
	const uint32 numYtiles = static_cast<int>(ceil(static_cast<float>(settings.height) / static_cast<float>(settings.tileSize)));
	const uint32 numTiles = numXtiles * numYtiles;

	Ray ray;
	Vec3 color;

	statistics.timer.start();

	while(1) {

		tileMutex.lock();
		uint32 tile = currentTile++;
		tileMutex.unlock();

		if (tile >= numTiles) break;

		const int tileOffsetX = settings.tileSize * (tile % numXtiles);
		const int tileOffsetY = settings.tileSize * (tile / numXtiles);

		for (uint32 y = tileOffsetY; y < tileOffsetY + settings.tileSize && y < settings.height; y++) {
			for (uint32 x = tileOffsetX; x < tileOffsetX + settings.tileSize && x < settings.width; x++) {

				color.set(0.0f);

				for (uint32 s = 0; s < settings.samples; ++s) {
					float u = static_cast<float>(x + Utils::random0To1()) / static_cast<float>(settings.width - 1.0f);
					float v = static_cast<float>(y + Utils::random0To1()) / static_cast<float>(settings.height - 1.0f);

					ray = scene.getCamera()->getRay(u, v);
					color += computeColor(ray, 0, statistics);
					statistics.primaryRays++;
				}

				color /= static_cast<float>(settings.samples);
				Vec3 srgb = Utils::rgbToSrgb(color);

				uint32 invertedY = settings.height - 1 - y;
				byte *ptr = imageBuffer + ((settings.width * invertedY) + x) * 3;
				*ptr++ = static_cast<byte>(255.999f * srgb.r);
				*ptr++ = static_cast<byte>(255.999f * srgb.g);
				*ptr = static_cast<byte>(255.999f * srgb.b);

				statistics.totalRenderedPixels++;
			}
		}
		statistics.totalRenderedTiles++;
	}
}

Vec3 PathTracer::computeColor(Ray &ray, uint32 depth, RenderStatistics &statistics) {

	HitRecord hitRecord;
	statistics.maxDepthReached = Utils::max(statistics.maxDepthReached, depth);

	if (scene.hit(ray, FLOAT_BIAS, FLT_MAX, hitRecord, statistics)) {

		Ray scattered;
		Vec3 attenuation;

		if (depth < settings.maxRayDepth) {
			if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
				statistics.scatteredRays++;
				return attenuation * computeColor(scattered, depth + 1, statistics);
			}
			else {
				statistics.absorbedRays++;
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
	for (uint32 i = 0; i < settings.threads; ++i) {
		std::cout << "----------------- Thread " << i << " -----------------" << std::endl;
		std::cout << threadStatistics[i] << std::endl;
		std::cout << "--------------------------------------------" << std::endl;
	}
}