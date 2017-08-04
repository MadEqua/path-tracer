#include "Timer.h"

Timer::Timer() :
	started(false) {
}

Timer::~Timer() {
}

void Timer::start() {
	startPoint = std::chrono::high_resolution_clock::now();
	started = true;
}

float Timer::getElapsedSeconds() const {
	if (started) {
		std::chrono::duration<float> dif = std::chrono::high_resolution_clock::now() - startPoint;
		uint64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(dif).count();
		return static_cast<float>(ms) / 1000.0f;
	}
	else
		return 0.0f;
}

uint32 Timer::getElapsedMilis() const
{
	if (started) {
		std::chrono::duration<float> dif = std::chrono::high_resolution_clock::now() - startPoint;
		uint64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(dif).count();
		return static_cast<uint32>(ms);
	}
	else
		return 0;
}