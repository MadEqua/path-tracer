#pragma once

#include <string>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

#include <glm/vec3.hpp>
#include "Types.h"
#include "RenderStatistics.h"

class Scene;
class Ray;

enum FileFormat {
	BMP, PNG, JPG, HDR
};

struct RenderSettings {
	uint32 width, height;
	uint32 samples;
	uint32 maxRayDepth;
	uint32 tileSize;
	uint32 threads;
	std::string outputFileName;
	FileFormat outputFileFormat;
};

class PathTracer
{
public:
	PathTracer(const RenderSettings &settings, Scene &scene);
	~PathTracer();

	void renderScene();

private:
	const RenderSettings &settings;
	Scene &scene;

	std::vector<std::thread> threadPool;
	std::vector<RenderStatistics> threadStatistics;

	std::mutex tileMutex;
	uint32 currentTile;
	void *imageBuffer; //Can be byte* or float* depending on render format

	void renderTile(int threadId);
	glm::vec3 computeColor(Ray &ray, uint32 depth, RenderStatistics &statistics);

	void printPreRender() const;
	void printPostRender() const;
};

inline std::ostream& operator<<(std::ostream &os, const RenderSettings &settings) {
	os << "File output: " << settings.outputFileName << std::endl <<
		"Dimensions: " << settings.width << "x" << settings.height << std::endl <<
		"Samples: " << settings.samples << std::endl <<
		"Max ray depth: " << settings.maxRayDepth << std::endl <<
		"Tile size: " << settings.tileSize << std::endl <<
		"Threads: " << settings.threads;
	return os;
}