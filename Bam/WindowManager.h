#pragma once

//#include "ReferenceManager.h"

struct UIrenderInfo;
struct CameraInfo;
class TextRenderer;
class BaseWindow;
class ConsoleTest;

class WindowManager
{
public:
	//ManagedReference<BaseWindow, ConsoleTest> consoleWindow;
	//ManagedReference<BaseWindow, BaseWindow> activeWindow;
	
	bool focussed;

	//void registerConsoleWindow(WeakReference<BaseWindow, ConsoleTest>& con);
	void focusConsoleWindow();

	void charPressed(char c);

	void unfocus();
	void focus();

	// (-1,1) range
	bool mouseCallback(glm::vec2 pos, int key, int action, int mods);
	bool key_callback(int key, int scancode, int action, int mods);

	void addRenderInfo(UIrenderInfo& uiRenderInfo, CameraInfo& cameraInfo, TextRenderer& textRenderer);

	WindowManager();
	~WindowManager();
};

