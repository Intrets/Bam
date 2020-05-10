#include "common.h"

#include "FPSdisplay.h"
#include "ReferenceManager.h"
//#include "TextWindow.h"
#include <string>

FPSDisplay::FPSDisplay() :
	nbFrames(0),
	fps(0),
	lastTime(glfwGetTime()) {
	//auto windowRefManager = Locator<ReferenceManager<BaseWindow>>::getService();
	//auto t = windowRefManager->makeRef<TextWindow>(glm::vec2(0.9, 0.9), glm::vec2(1, 1));
	//fpsDisplayWindow.set(t);

}

FPSDisplay::~FPSDisplay() {
}

void FPSDisplay::displayFPS(GLFWwindow* window) {
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) {
		fps = nbFrames;
		nbFrames = 0;
		lastTime = currentTime;

		auto fpsmode2 = FPS_WINDOW;
		switch (fpsmode2) {
			case FPS_NULL:
				break;
			case FPS_SCREEN:
				//if (fpsDisplayWindow.isValid()) {
				//	fpsDisplayWindow.get()->text = std::to_string(fps);
				//}
				break;
			case FPS_WINDOW:
				if (nbFrames == 0) {
					std::string fps_string = "fps: " + std::to_string(fps);
					char const* text = fps_string.c_str();
					glfwSetWindowTitle(window, text);
				}
				break;
		}
	}
}



FPSdisplay::FPSdisplay() {
}


FPSdisplay::~FPSdisplay() {
}
