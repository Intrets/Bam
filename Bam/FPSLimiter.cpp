#include "common.h"

#include "FPSLimiter.h"


FPSLimiter::FPSLimiter() :
	renderFrameTimeTarget(1 / 60.0),
	lastRenderTimes(renderFrameTimeTarget, 20),
	previousFrameEnd(glfwGetTime()),
	previousFrameStart(glfwGetTime()) {
}

FPSLimiter::~FPSLimiter() {
}

void FPSLimiter::loopStart() {
}

void FPSLimiter::loopFinish() {
}

void FPSLimiter::renderStart() {
	previousFrameStart = glfwGetTime();
}

void FPSLimiter::renderFinish() {
	previousFrameEnd = glfwGetTime();
	lastRenderTimes.insert(previousFrameEnd - previousFrameStart);
}

bool FPSLimiter::ready() {
	return getTimeTillNextRender() <= 0.0;
}

double FPSLimiter::getTimeTillNextRender() {
	double remaining = 0.0;
	switch (frameSyncMode) {
		case OFF:
			remaining = 0.0;
			break;
		case VSYNC:
			remaining = 0.0;
			break;
		case LIMITING:
			remaining = previousFrameEnd + renderFrameTimeTarget - (glfwGetTime() + lastRenderTimes.average(0.0));
			break;
	}
	return remaining;
}
