#pragma once

#include "Option.h"
//#include "BaseWindow.h"
//#include "TextWindow.h"
#include "ReferenceManager.h"

class OptionInteger;

typedef enum
{
	FPS_NULL,
	FPS_SCREEN,
	FPS_WINDOW,
} FPS_display_mode;

class FPSDisplay
{
public:
	double lastTime;

	int32_t nbFrames;
	int32_t fps;

	//ManagedReference<BaseWindow, TextWindow> fpsDisplayWindow;
	//Option<int> fpsmode2{ "fps_displaymode", 1 };

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


