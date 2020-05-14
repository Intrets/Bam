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
	bool mouseCallback(glm::vec2 pos, int32_t key, int32_t action, int32_t mods);
	bool key_callback(int32_t key, int32_t scancode, int32_t action, int32_t mods);

	void addRenderInfo(UIrenderInfo& uiRenderInfo, CameraInfo& cameraInfo, TextRenderer& textRenderer);

	WindowManager();
	~WindowManager();
};

