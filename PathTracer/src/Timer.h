#pragma once

#include <chrono>
#include "Types.h"

class Timer
{
public:
	Timer();

	void start();
	float getElapsedSeconds() const;
	uint32 getElapsedMilis() const;

private:
	bool started;
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
};