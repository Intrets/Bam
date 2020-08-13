#pragma once

#include "CircularArray.h"

typedef enum
{
	OFF,
	VSYNC,
	LIMITING
} FRAME_SYNC_OPTION;

class FPSLimiter
{
public:
	FPSLimiter();
	~FPSLimiter();

	void loopStart();
	void loopFinish();

	void renderStart();
	void renderFinish();

	bool ready();

private:
	// Need to initialize in initialization list
	CircularArray<double> lastRenderTimes;

	double previousFrameStart;
	double previousFrameEnd;

	double renderFrameTimeTarget;
	double getTimeTillNextRender();

	FPSLimiter(const FPSLimiter&) = delete;
	FPSLimiter(FPSLimiter&&) = delete;
	FPSLimiter& operator= (const FPSLimiter&) = delete;
	FPSLimiter& operator= (FPSLimiter&&) = delete;
};


