#include "common.h"

#include "FPSLimiter.h"


FPSLimiter::FPSLimiter() :
	renderFrameTimeTarget(1 / 60.0),
	lastRenderTimes(this->renderFrameTimeTarget, 20),
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
	this->previousFrameStart = glfwGetTime();
}

void FPSLimiter::renderFinish() {
	this->previousFrameEnd = glfwGetTime();
	this->lastRenderTimes.insert(this->previousFrameEnd - this->previousFrameStart);
}

bool FPSLimiter::ready() {
	return this->getTimeTillNextRender() <= 0.0;
}

double FPSLimiter::getTimeTillNextRender() {
	double remaining = 0.0;
	switch (Option<OPTION::GR_FRAMESYNCMODE, int>::getVal()) {
		case OFF:
			remaining = 0.0;
			break;
		case VSYNC:
			remaining = 0.0;
			break;
		case LIMITING:
			remaining = this->previousFrameEnd + this->renderFrameTimeTarget - (glfwGetTime() + this->lastRenderTimes.average(0.0));
			break;
	}
	return remaining;
}
