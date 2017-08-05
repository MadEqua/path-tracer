#pragma once

#include <string>
#include <iostream>
#include "Types.h"
#include "Vec3.h"
#include "RenderStatistics.h"

class Scene;
class Ray;

struct RenderSettings {
	uint32 width, height;
	uint32 samples;
	uint32 maxRayDepth;
	std::string outputFileName;
};

class PathTracer
{
public:
	PathTracer(const RenderSettings &settings, Scene &scene);
	~PathTracer();

	void renderScene();
	void printPreRender() const;
	void printPostRender() const;

private:
	const RenderSettings &settings;
	Scene &scene;
	RenderStatistics statistics;

	Vec3 computeColor(Ray &ray, uint32 depth);
};

inline std::ostream& operator<<(std::ostream &os, const RenderSettings &settings) {
	os << "File output: " << settings.outputFileName << std::endl <<
		"Dimensions: " << settings.width << "x" << settings.height << std::endl <<
		"Samples: " << settings.samples << std::endl <<
		"Max ray depth: " << settings.maxRayDepth;
	return os;
}