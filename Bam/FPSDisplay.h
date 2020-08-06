#pragma once

#include "Option.h"
#include "ReferenceManager.h"

class OptionInteger;

typedef enum
{
	FPS_NULL,
	FPS_SCREEN,
	FPS_WINDOW,
} FPS_display_mode;

// TODO: rework
class FPSDisplay
{
public:
	double lastTime;

	int32_t nbFrames;
	int32_t fps;

	FPSDisplay();
	~FPSDisplay();

	void displayFPS(GLFWwindow* window);
};

class FPSdisplay
{
public:
	FPSdisplay();
	~FPSdisplay();
};


