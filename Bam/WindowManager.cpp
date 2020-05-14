#include "common.h"
#include "WindowManager.h"
//#include "BaseWindow.h"

//void WindowManager::registerConsoleWindow(WeakReference<BaseWindow, ConsoleTest>& con) {
//	consoleWindow.set(con);
//}

void WindowManager::focusConsoleWindow() {
	//if (consoleWindow.isValid()) {
	//	focussed = true;
	//	activeWindow.set(consoleWindow.handle);
	//}
}

void WindowManager::charPressed(char c) {
	//if (focussed && activeWindow.isValid()) {
	//	activeWindow.get()->charPressed(c);
	//}
}

void WindowManager::unfocus() {
	focussed = false;
}

void WindowManager::focus() {
	focussed = true;
}

bool WindowManager::mouseCallback(glm::vec2 pos, int32_t key, int32_t action, int32_t mods) {
	//activeWindow.unset();
	//unfocus();
	//auto ref = Locator<ReferenceManager<BaseWindow>>::getService();
	//for (auto& pair : ref->data) {
	//	auto& window = pair.second;
	//	Handle handle = pair.first;
	//	if (window.get()->isClickedOn(pos)) {
	//		activeWindow.set(handle);
	//		focus();
	//		window.get()->mouseCallback(pos, key, action, mods);
	//		break;
	//	}
	//}
	return focussed;
}

bool WindowManager::key_callback(int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	//if (focussed && activeWindow.isValid()) {
	//	activeWindow.get()->key_callback(key, scancode, action, mods);
	//	return true;
	//}
	return false;
}

void WindowManager::addRenderInfo(UIrenderInfo & uiRenderInfo, CameraInfo & cameraInfo, TextRenderer & textRenderer) {
	//auto ref = Locator<ReferenceManager<BaseWindow>>::getService();
	//for (auto& pair : ref->data) {
	//	auto& window = pair.second;
	//	Handle handle = pair.first;
	//	window.get()->addRenderInfo(uiRenderInfo, cameraInfo, textRenderer);
	//}
}

WindowManager::WindowManager() {
}


WindowManager::~WindowManager() {
}
