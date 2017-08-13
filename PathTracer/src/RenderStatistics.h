#pragma once

#include "Types.h"
#include "Timer.h"
#include <cstring>

struct RenderStatistics {
	uint32 totalRenderedPixels;
	uint32 totalRenderedTiles;
	uint32 primaryRays; //This should be width * height * samples
	uint32 scatteredRays;
	uint32 absorbedRays;
	uint32 maxDepthReached; //The maximum depth reached by any ray
	uint32 raysReachedMaxDepth; //The count of rays that reached the max depth from RenderSettings
	uint32 bvhIntersectionsComputed;
	uint32 bvhIntersectionsFailed;
	uint32 objectIntersectionsComputed;
	uint32 objectIntersectionsFailed;
	Timer timer;

	RenderStatistics() {
		memset(this, 0, sizeof(RenderStatistics) - sizeof(Timer));
	}
};

inline std::ostream& operator<<(std::ostream &os, const RenderStatistics &stats) {
	os << "Total rendered pixels: " << stats.totalRenderedPixels << std::endl <<
		"Total rendered tiles: " << stats.totalRenderedTiles << std::endl <<
		"Primary rays: " << stats.primaryRays << std::endl <<
		"Scattered rays: " << stats.scatteredRays << std::endl <<
		"Absorbed rays: " << stats.absorbedRays << std::endl <<
		"Max depth reached by any ray: " << stats.maxDepthReached << std::endl <<
		"Rays that reached the settings max depth: " << stats.raysReachedMaxDepth << std::endl <<
		"BVH intersections computed: " << stats.bvhIntersectionsComputed <<  " (" << stats.bvhIntersectionsFailed << " failed)" << std::endl <<
		"Object intersections computed: " << stats.objectIntersectionsComputed << " (" << stats.objectIntersectionsFailed << " failed)" << std::endl <<
		"Time elapsed: " << stats.timer.getElapsedSeconds() << " seconds";
	return os;
}
