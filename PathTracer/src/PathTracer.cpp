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
#include "BloomPostProcess.h"
#include "ToneMapper.h"
#include "Sky.h"

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
	delete[] imageBuffer;
}

void PathTracer::renderScene() {
	
	printPreRender();
	scene.initializeStaticData();

	imageBuffer = new float[settings.width * settings.height * 3];

	std::cout << "Generating some pretty pixels... :)";
	
	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i] = std::thread(&PathTracer::renderTile, this, i);
	}

	for (uint32 i = 0; i < settings.threads; ++i) {
		threadPool[i].join();
	}

	std::cout << " Done! " << std::endl;

	applyPostProcessing();
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
				Utils::setPixelToImage(imageBuffer, settings.width, settings.height, x, y, color);
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

	const Sky *sky = scene.getSky();
	if (sky != nullptr) {
		glm::vec3 dir = glm::normalize(ray.direction);
		return sky->getColor(dir);
	}

	return glm::vec3(0.0f);

	//float t = 0.5f * (dir.y + 1.0f);
	//return (1.0f - t) * glm::vec3(0.0f, 0.0f, 0.1f) + t * glm::vec3(0.25f, 0.46f, 0.78f);
	//return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

void PathTracer::applyPostProcessing() {

	std::cout << "Applying bloom post-processing..." << std::endl;
	
	float *postProcessOutput = new float[settings.width * settings.height * 3];
	BloomPostProcess postProcess(imageBuffer, postProcessOutput, settings.width, settings.height,
		settings.gaussianKernelSize, settings.gaussianKernelSize,
		settings.gaussianSigma, settings.luminosityThreshold);
	postProcess.process();

	if (settings.outputFileFormat != FileFormat::HDR) {

		byte *byteBuffer = new byte[settings.width * settings.height * 3];
		ToneMapper toneMapper(postProcessOutput, settings.width, settings.height, settings.key, settings.lumWhite);
		
		for (uint32 y = 0; y < settings.height; ++y) {
			for (uint32 x = 0; x < settings.width; ++x) {
				glm::vec3 color = Utils::getPixelFromImage(postProcessOutput, settings.width, settings.height, x, y);
				glm::u8vec3 byteColor = toneMapper.mapColor(color);
				Utils::setPixelToImage(byteBuffer, settings.width, settings.height, x, y, byteColor);
			}
		}

		switch (settings.outputFileFormat)
		{
		case FileFormat::BMP:
			stbi_write_bmp((settings.outputFileName + ".bmp").c_str(), settings.width, settings.height, 3, byteBuffer);
			break;
		case FileFormat::JPG:
			stbi_write_jpg((settings.outputFileName + ".jpg").c_str(), settings.width, settings.height, 3, byteBuffer, 100);
			break;
		case FileFormat::PNG:
		default:
			stbi_write_png((settings.outputFileName + ".png").c_str(), settings.width, settings.height, 3, byteBuffer, 0);
			break;
		}

		//useful to debug tonemapping
		//stbi_write_hdr((settings.outputFileName + ".hdr").c_str(), settings.width, settings.height, 3, postProcessOutput);

		delete[] byteBuffer;
	}
	else {
		stbi_write_hdr((settings.outputFileName + ".hdr").c_str(), settings.width, settings.height, 3, postProcessOutput);
	}

	delete[] postProcessOutput;
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

std::ostream& operator<<(std::ostream &os, const RenderSettings &settings) {
	os << "File output: " << settings.outputFileName << std::endl <<
		"Dimensions: " << settings.width << "x" << settings.height << std::endl <<
		"Samples: " << settings.samples << std::endl <<
		"Max ray depth: " << settings.maxRayDepth << std::endl <<
		"Tile size: " << settings.tileSize << std::endl <<
		"Threads: " << settings.threads << std::endl <<
		"[Bloom Post-Processing]:" << std::endl <<
		"Luminosity Threshold: " << settings.luminosityThreshold << std::endl <<
		"Gaussian Kernel size: " << settings.gaussianKernelSize << std::endl <<
		"Gaussian Sigma: " << settings.gaussianSigma << std::endl <<
		"[Tone-Mapping]:" << std::endl <<
		"Key: " << settings.key << std::endl <<
		"Luminance mapped to white: " << settings.lumWhite;
	return os;
}