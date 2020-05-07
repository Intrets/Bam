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

void mainLoop(GLFWwindow* window, ControlState& controlState) {
	FPSLimiter fpsLimiter;
	WindowManager windowManager;

	GameState gameState;
	GameLogic gameLogic;

	Renderer renderer;

	std::thread logicThread;

	int fps = 0;
	double last = 0;

	while (!glfwWindowShouldClose(window)) {
		//if (gamestate.exit) {
		//	glfwSetWindowShouldClose(window, GLFW_TRUE);
		//	break;
		//}

		RenderInfo renderInfo;

		bool rendering = fpsLimiter.ready();
		bool logic = gameLogic.ready();

		if (rendering) {
			renderer.prepareRender(window, renderInfo, gameState, windowManager);
		}

		if (logic) {
			controlState.cycleStates();
			glfwPollEvents();

			gameState.updatePlayerCursorScreenSpace(window);

			Locator<BindHandler>::getService()->runBinds(controlState, gameState);

			logicThread = std::thread(&GameLogic::runStep, &gameLogic, std::ref(gameState));
		}

		if (rendering) {
			if (glfwGetTime() - last > 1.0f) {
				last = glfwGetTime();
				std::cout << fps << "\n";
				fps = 0;
			}
			fps++;

			fpsLimiter.renderStart();
			renderer.render(window, renderInfo);
			fpsLimiter.renderFinish();
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