#include "common.h"

#include "Main.h"
#include "GameLogic.h"
#include "Renderer.h"
#include "FPSLimiter.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "ControlState.h"
#include <thread>
#include "FPSDisplay.h"
#include "UIState.h"

void mainLoop(GLFWwindow* window, ControlState& controlState) {
	FPSLimiter fpsLimiter;
	FPSDisplay fpsDisplay;

	GameState gameState;
	GameLogic gameLogic;

	UIState uiState;
	uiState.updateSize(window);

	Renderer renderer;

	std::thread logicThread;

	while (!glfwWindowShouldClose(window)) {
		RenderInfo renderInfo;

		bool rendering = fpsLimiter.ready();

		if (rendering) {
			uiState.updateSize(window);
			renderer.prepareRender(window, renderInfo, gameState, uiState);
		}

		bool logic = gameLogic.ready();
		if (logic) {
			controlState.cycleStates();
			glfwPollEvents();

			gameState.updatePlayerCursorScreenSpace(window);

			uiState.run(gameState, controlState);

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