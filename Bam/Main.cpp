#include "common.h"

#include "Main.h"
#include "GameLogic.h"
#include "Renderer.h"
#include "FPSLimiter.h"
#include "WindowManager.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "ControlState.h"
#include <thread>
#include "BindHandler.h"
#include "FPSDisplay.h"

void mainLoop(GLFWwindow* window, ControlState& controlState) {
	FPSLimiter fpsLimiter;
	FPSDisplay fpsDisplay;
	WindowManager windowManager;

	GameState gameState;
	GameLogic gameLogic;

	Renderer renderer;

	std::thread logicThread;

	while (!glfwWindowShouldClose(window)) {
		RenderInfo renderInfo;

		bool rendering = fpsLimiter.ready();

		if (rendering) {
			renderer.prepareRender(window, renderInfo, gameState, windowManager);
		}

		bool logic = gameLogic.ready();
		if (logic) {
			controlState.cycleStates();
			glfwPollEvents();

			gameState.updatePlayerCursorScreenSpace(window);

			Locator<BindHandler>::getService()->runBinds(controlState, gameState);

			logicThread = std::thread(&GameLogic::runStep, &gameLogic, std::ref(gameState));
		}

		if (rendering) {
			fpsLimiter.renderStart();
			renderer.render(window, renderInfo);
			fpsLimiter.renderFinish();
			fpsDisplay.displayFPS(window);
		}

		if (logicThread.joinable()) {
			logicThread.join();
		}

		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << std::hex << err << std::dec << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::microseconds((long) 1000));
	}
}