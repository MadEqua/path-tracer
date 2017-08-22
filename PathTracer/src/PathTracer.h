#pragma once

#include <iostream>
#include <string>
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
	
	//Threading
	uint32 tileSize;
	uint32 threads;

	//Bloom Post-Processing
	float luminosityThreshold;
	uint32 gaussianKernelSize;
	float gaussianSigma;

	//Tone-Mapping for Non-HDR formats
	float key;
	float lumWhite; //smallest luminance mapped to pure white

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
	float *imageBuffer;

	void renderTile(int threadId);
	glm::vec3 computeColor(Ray &ray, uint32 depth, RenderStatistics &statistics);
	void applyPostProcessing();

	void printPreRender() const;
	void printPostRender() const;
};

std::ostream& operator<<(std::ostream &os, const RenderSettings &settings);