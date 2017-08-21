#include "PathTracer.h"

#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Ray.h"
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
	threadStatistics(settings.threads),
	currentTile(0) {
}

PathTracer::~PathTracer() {
	if (settings.outputFileFormat == FileFormat::HDR)
		delete[] static_cast<float*>(imageBuffer);
	else
		delete[] static_cast<byte*>(imageBuffer);
}

void PathTracer::renderScene() {
	
	printPreRender();
	scene.initializeStaticData();

	if (settings.outputFileFormat == FileFormat::HDR)
		imageBuffer = new float[settings.width * settings.height * 3];
	else
		imageBuffer = new byte[settings.width * settings.height * 3];

	std::cout << "Coloring the pretty pixels... :)" << std::endl;
	
	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i] = std::thread(&PathTracer::renderTile, this, i);
	}

	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i].join();
	}

	switch (settings.outputFileFormat)
	{
	case FileFormat::BMP:
		stbi_write_bmp((settings.outputFileName + ".bmp").c_str(), settings.width, settings.height, 3, static_cast<byte*>(imageBuffer));
		break;
	case FileFormat::JPG:
		stbi_write_jpg((settings.outputFileName + ".jpg").c_str(), settings.width, settings.height, 3, static_cast<byte*>(imageBuffer), 100);
		break;
	case FileFormat::HDR:
		stbi_write_hdr((settings.outputFileName + ".hdr").c_str(), settings.width, settings.height, 3, static_cast<float*>(imageBuffer));
		break;
	case FileFormat::PNG:
	default:
		stbi_write_png((settings.outputFileName + ".png").c_str(), settings.width, settings.height, 3, static_cast<byte*>(imageBuffer), 0);
		break;
	}

	printPostRender();
}

void PathTracer::renderTile(int threadId) {
	
	RenderStatistics &statistics = threadStatistics[threadId];
	
	const uint32 numXtiles = static_cast<int>(ceil(static_cast<float>(settings.width) / static_cast<float>(settings.tileSize)));
	const uint32 numYtiles = static_cast<int>(ceil(static_cast<float>(settings.height) / static_cast<float>(settings.tileSize)));
	const uint32 numTiles = numXtiles * numYtiles;

	Ray ray;
	glm::vec3 color;

	statistics.timer.start();

	while(1) {

		tileMutex.lock();
		uint32 tile = currentTile++;
		tileMutex.unlock();

		if (tile >= numTiles) break;

		const int tileOffsetX = settings.tileSize * (tile % numXtiles);
		const int tileOffsetY = settings.tileSize * (tile / numXtiles);

		const float inverseSamples = 1.0f / static_cast<float>(settings.samples);

		for (uint32 y = tileOffsetY; y < tileOffsetY + settings.tileSize && y < settings.height; y++) {
			for (uint32 x = tileOffsetX; x < tileOffsetX + settings.tileSize && x < settings.width; x++) {

				color.r = color.g = color.b = 0.0f;

				for (uint32 s = 0; s < settings.samples; ++s) {
					float u = static_cast<float>(x + glm::linearRand(0.0f, 1.0f)) / static_cast<float>(settings.width - 1.0f);
					float v = static_cast<float>(y + glm::linearRand(0.0f, 1.0f)) / static_cast<float>(settings.height - 1.0f);

					ray = scene.getCamera()->getRay(u, v);
					color += computeColor(ray, 0, statistics);
					statistics.primaryRays++;
				}

				color *= inverseSamples;

				uint32 invertedY = settings.height - 1 - y;
				if (settings.outputFileFormat == FileFormat::HDR) {
					float *ptr = static_cast<float*>(imageBuffer) + ((settings.width * invertedY) + x) * 3;
					*ptr++ = color.r;
					*ptr++ = color.g;
					*ptr = color.b;
				}
				else {				
					glm::vec3 srgb = Utils::rgbToSrgb(color);
					color = glm::clamp(color, 0.0f, 1.0f);

					byte *ptr = static_cast<byte*>(imageBuffer) + ((settings.width * invertedY) + x) * 3;
					*ptr++ = static_cast<byte>(255.999f * srgb.r);
					*ptr++ = static_cast<byte>(255.999f * srgb.g);
					*ptr = static_cast<byte>(255.999f * srgb.b);
				}

				statistics.totalRenderedPixels++;
			}
		}
		statistics.totalRenderedTiles++;
	}
}

glm::vec3 PathTracer::computeColor(Ray &ray, uint32 depth, RenderStatistics &statistics) {

	HitRecord hitRecord;
	statistics.maxDepthReached = Utils::max(statistics.maxDepthReached, depth);

	if (scene.hit(ray, FLOAT_BIAS, INF_FLOAT32, hitRecord, statistics)) {

		Ray scattered;
		glm::vec3 attenuation;

		if (depth < settings.maxRayDepth) {
		
			glm::vec3 emission = hitRecord.material->emit(hitRecord.u, hitRecord.v);
			
			if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
				statistics.scatteredRays++;
				//return (hitRecord.normal + 1.0f) / 2.0f; //Normal test
				return emission + attenuation * computeColor(scattered, depth + 1, statistics);
			}
			else {
				statistics.absorbedRays++;
				return emission;
			}
		}
		else {
			statistics.raysReachedMaxDepth++;
			return glm::vec3(0.0f);
		}
	}

	return glm::vec3(0.0f);

	//glm::vec3 dir = glm::normalize(ray.direction);
	//float t = 0.5f * (dir.y + 1.0f);
	//return (1.0f - t) * glm::vec3(0.0f, 0.0f, 0.1f) + t * glm::vec3(0.25f, 0.46f, 0.78f);

	//return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

void PathTracer::printPreRender() const {
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << "Rendering " << scene.getObjectCount() << " objects" << std::endl;
	std::cout << settings << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
}

void PathTracer::printPostRender() const {
	std::cout << "Finished!" << std::endl << std::endl;

	for (uint32 i = 0; i < settings.threads; ++i) {
		std::cout << "----------------- Thread " << i << " -----------------" << std::endl;
		std::cout << threadStatistics[i] << std::endl;
		std::cout << "--------------------------------------------" << std::endl;
	}
}