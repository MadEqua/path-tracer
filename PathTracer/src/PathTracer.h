#pragma once

#include <string>
#include "Types.h"
#include "Vec3.h"

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
	PathTracer(const RenderSettings &settings, const Scene &scene);
	~PathTracer();

	void renderScene() const;

private:
	const RenderSettings &settings;
	const Scene &scene;

	Vec3 computeColor(Ray &ray, uint32 depth) const;
};

